#ifndef DECODER_RSC_BCJR_HPP_
#define DECODER_RSC_BCJR_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../../Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RSC_BCJR : public Decoder_SISO<B,R>
{
protected:
	const int  n_states;
	const int  n_ff;
	const bool buffered_encoding;

	const std::vector<std::vector<int>> &trellis;

	mipp::vector<R> sys, par; // input LLR from the channel
	mipp::vector<R> ext;      // extrinsic LLRs
	mipp::vector<B> s;        // hard decision

public:
	Decoder_RSC_BCJR(const int K,
	                 const std::vector<std::vector<int>> &trellis,
	                 const bool buffered_encoding = true,
	                 const int n_frames = 1,
	                 const int simd_inter_frame_level = 1,
	                 const std::string name = "Decoder_RSC_BCJR");
	virtual ~Decoder_RSC_BCJR();

	virtual int tail_length() const { return 2 * n_ff; }
	virtual void soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext) = 0;

protected:
	virtual void load        (const mipp::vector<R>& Y_N);
	        void _hard_decode(                          );
	virtual void store       (      mipp::vector<B>& V_K) const;

	virtual void _soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2);
};
}
}

#include "Decoder_RSC_BCJR.hxx"

#endif /* DECODER_RSC_BCJR_HPP_ */
