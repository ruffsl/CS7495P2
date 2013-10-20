#include "Image.h"

namespace cs7495
{

	/// <summary>
	/// Class to extract frames from a video and associate
	/// a GPS location from a KML file.
	/// </summary>
	class Extractor
	{
	private:
		std::vector<Image> frames;	///< Contains all the frames in the video
		time_t timestamp;

	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Extractor();

		/// <summary>
		/// Extract all frames from a video file.
		/// </summary>
		/// <param name="filepath">[in] Path to the video file</param>
		void breakVideo(const std::string& filepath);

		/// <summary>
		/// Extract each frame and saves it as a JPEG file
		/// without keeping it in memory.
		/// </summary>
		/// <param name="filepath">[in] Path to the video file</param>
		void video2images(const std::string& filepath);


		/// <summary>
		/// Extract time from and MP4 file name
		/// </summary>
		/// <param name="filepath">[in] Path to the video file</param>
		void getTimeName(const std::string& filepath);
	};
}
