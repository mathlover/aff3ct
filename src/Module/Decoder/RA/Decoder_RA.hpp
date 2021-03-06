#ifndef DECODER_RA
#define DECODER_RA

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Decoder.hpp"
#include "../../Interleaver/Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RA : public Decoder<B, R>
{
protected:
	const int rep_count; // number of repetitions
	int max_iter;        // max number of iterations

	mipp::vector<R> Y_N, Fw, Bw;
	mipp::vector<B> V_K;
	mipp::vector<R> Tu, Td, Wu, Wd, U;
	std::vector<mipp::vector<R>> Xd, Xu;

	Interleaver<int>& interleaver;

public:
	Decoder_RA(const int& K, const int& N, Interleaver<int>& interleaver, int max_iter,
	           const int n_frames = 1, const std::string name = "Decoder_RA");
	virtual ~Decoder_RA();

protected:
	void load        (const mipp::vector<R>& Y_N);
	void _hard_decode(                          );
	void store       (      mipp::vector<B>& V_K) const;

private:
	R check_node(R a, R b);
	int sign(R x);
};
}
}

#endif /* DECODER_RA */
