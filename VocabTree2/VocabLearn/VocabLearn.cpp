/* 
 * Copyright 2011-2012 Noah Snavely, Cornell University
 * (snavely@cs.cornell.edu).  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:

 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY NOAH SNAVELY ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL NOAH SNAVELY OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * 
 * The views and conclusions contained in the software and
 * documentation are those of the authors and should not be
 * interpreted as representing official policies, either expressed or
 * implied, of Cornell University.
 *
 */

/* VocabLearn.cpp */
/* Driver for learning a vocabulary tree through hierarchical kmeans */

#include <string>
#include <vector>
#include <string.h>
#include <time.h>
#include <fstream>

#include "VocabTree.h"
#include "keys2.h"
#include "defines.h"

#define MAX_ARRAY_SIZE 8388608 // 2 ** 23

int main(int argc, char **argv) 
{
    if (argc != 6) {
        printf("Usage: %s <list.in> <depth> <branching_factor> "
               "<restarts> <tree.out>\n", argv[0]);
        return 1;
    }

    const char *list_in = argv[1];
    int depth = atoi(argv[2]);
    int bf = atoi(argv[3]);
    int restarts = atoi(argv[4]);
    const char *tree_out = argv[5];
    std::ofstream log("debug_VocabLearn.txt");
    log << "Building tree with depth: " << depth << ", branching factor: " << bf << ", and restarts: " << restarts << std::endl;
    printf("Building tree with depth: %d, branching factor: %d, "
           "and restarts: %d\n", depth, bf, restarts);

    FILE *f = fopen(list_in, "r");
    
    std::vector<std::string> key_files;
    char buf[256];
    while (fgets(buf, 256, f)) {
        /* Remove trailing newline */
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0;

        key_files.push_back(std::string(buf));
    }

    fclose(f);

    int num_files = (int) key_files.size();
    unsigned long total_keys = 0;
    for (int i = 0; i < num_files; i++) {
        int num_keys = GetNumberOfKeys(key_files[i].c_str());
        total_keys += num_keys;
    }

    log << "Total number of keys: " << total_keys << std::endl;
    printf("Total number of keys: %lu\n", total_keys);
    fflush(stdout);

    int dim = 128;
    unsigned long long len = (unsigned long long) total_keys * dim;

    int num_arrays = 
        len / MAX_ARRAY_SIZE + ((len % MAX_ARRAY_SIZE) == 0 ? 0 : 1);

    unsigned char **vs = new unsigned char *[num_arrays];

    log << "Allocating " << len << " bytes in total, in " << num_arrays << " arrays" << std::endl;
    printf("Allocating %llu bytes in total, in %d arrays\n", len, num_arrays);

    unsigned long long total = 0;
    for (int i = 0; i < num_arrays; i++) {
        unsigned long long remainder = len - total;
        unsigned long len_curr = MIN(remainder, MAX_ARRAY_SIZE);

        log << "Allocating array of size " << len_curr << std::endl;
        printf("Allocating array of size %lu\n", len_curr);
        fflush(stdout);
        vs[i] = new unsigned char[len_curr];

        remainder -= len_curr;
    }

    /* Create the array of pointers */
    log << "Allocating pointer array of size " << sizeof(unsigned char) * total_keys << std::endl;
    printf("Allocating pointer array of size %lu\n", 4 * total_keys);
    fflush(stdout);

    unsigned char **vp = new unsigned char *[total_keys];
    
    unsigned long off = 0;
    unsigned long curr_key = 0;
    int curr_array = 0;
    for (int i = 0; i < num_files; i++) {
    	log << "Reading key file " << key_files[i] << std::endl;
        //printf("  Reading keyfile %s\n", key_files[i].c_str());
        fflush(stdout);

        unsigned char *keys;
        int num_keys = 0;

        keypt_t *info = NULL;
        num_keys = ReadKeyFileCXX(key_files[i].c_str(), &keys);
        log << "There are " << num_keys << " keys in file " << key_files[i] << std::endl;
//        num_keys = ReadKeyFile(key_files[i].c_str(), &keys);

#ifdef DEBUG
        std::ofstream debug_log("debug_log_2.txt");
        debug_log << num_keys << " " << dim << std::endl;
        for (int i=0; i<num_keys; i++) {
        	for (int k=0; k<dim; k++) {
        		debug_log << (int) keys[dim*i + k] << " ";
        	}
        	debug_log << std::endl;
        }
        debug_log.close();
#endif

        if (num_keys > 0) {
            for (int j = 0; j < num_keys; j++) {
#ifdef DEBUG
            	log << "Copying key " << j << std::endl;
#endif
                for (int k = 0; k < dim; k++) {
#ifdef DEBUG
                	log << "\tReading ";
#endif
                	unsigned char val = keys[j * dim + k];
#ifdef DEBUG
                	log << (int) val << ". Writing ";
#endif
                    vs[curr_array][off + k] = val;
#ifdef DEBUG
                    log << (int) vs[curr_array][off + k] << std::endl;
#endif
                }
#ifdef DEBUG
                log << "vp[curr_key] = vs[curr_array] + off;" << std::endl;
#endif
                vp[curr_key] = vs[curr_array] + off;
                curr_key++;
                off += dim;
                if (off == MAX_ARRAY_SIZE) {
                    off = 0;
                    curr_array++;
                }
            }

            delete [] keys;

            if (info != NULL)
                delete [] info;
        }
    }

    log << "Building tree..." << std::endl;
    VocabTree tree;
    tree.Build(total_keys, dim, depth, bf, restarts, vp);
    tree.Write(tree_out);

    log.close();

    return 0;
}
