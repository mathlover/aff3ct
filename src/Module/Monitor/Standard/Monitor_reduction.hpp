#ifndef MONITOR_REDUCTION_HPP_
#define MONITOR_REDUCTION_HPP_

#include <string>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"
#include "Monitor_std.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Monitor_reduction : public Monitor_std<B,R>
{
private:
	std::vector<Monitor<B,R>*>& monitors;
	const std::string           error_tracker_head_file_name;

public:
	Monitor_reduction(const int& K, const int& N, const unsigned& max_fe, std::vector<Monitor<B,R>*>& monitors,
	                  const int& n_frames = 1, const std::string name = "Monitor_reduction");
	virtual ~Monitor_reduction();

	unsigned long long get_n_analyzed_fra() const;
	unsigned long long get_n_fe          () const;
	unsigned long long get_n_be          () const;

	void dump_bad_frames(const std::string& base_path, const float snr, const mipp::vector<int>& itl_pi = mipp::vector<int>(0));

	static bool check_path(const std::string& base_path); // return true if correct
	static void get_tracker_paths(const std::string& base_path, const float snr, std::string& path_src,
	                              std::string& path_enc, std::string& path_noise, std::string& path_itl);
};
}
}

#endif /* MONITOR_REDUCTION_HPP_ */
