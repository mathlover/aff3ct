#ifndef CHANNEL_RAYLEIGH_LLR_HPP_
#define CHANNEL_RAYLEIGH_LLR_HPP_

#include <random>

#include "../Channel.hpp"

namespace aff3ct
{
template <typename R>
class Channel_Rayleigh_LLR : public Channel<R> // flat Rayleigh fading channel
{
private:
	const R    sigma;
	const bool complex;

	std::random_device          rd;
	std::mt19937                rd_engine;
	std::normal_distribution<R> normal_dist_n;
	std::normal_distribution<R> normal_dist_h;

public:
	Channel_Rayleigh_LLR(const int N, const R& sigma, const bool complex, const int seed = 0, const int n_frames = 1,
	                     const std::string name = "Channel_Rayleigh_LLR");
	virtual ~Channel_Rayleigh_LLR();

	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N);
	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N, mipp::vector<R> &H_N);
};
}

#endif /* CHANNEL_RAYLEIGH_LLR_HPP_ */
