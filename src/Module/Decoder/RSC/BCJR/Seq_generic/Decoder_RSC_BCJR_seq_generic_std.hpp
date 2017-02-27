#ifndef DECODER_RSC_BCJR_STD_GENERIC_SEQ_HPP_
#define DECODER_RSC_BCJR_STD_GENERIC_SEQ_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Math/max.h"

#include "Decoder_RSC_BCJR_seq_generic.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename RD = float,
          tools::proto_max<R> MAX1 = tools::max, tools::proto_max<RD> MAX2 = tools::max>
class Decoder_RSC_BCJR_seq_generic_std : public Decoder_RSC_BCJR_seq_generic<B,R>
{
private:
	mipp::vector<R> beta_prev;
	mipp::vector<R> beta_cur;
	
public:
	Decoder_RSC_BCJR_seq_generic_std(const int &K, 
	                                 const std::vector<std::vector<int>> &trellis, 
	                                 const bool buffered_encoding = true,
	                                 const int n_frames = 1,
	                                 const std::string name = "Decoder_RSC_BCJR_seq_generic_std");
	virtual ~Decoder_RSC_BCJR_seq_generic_std();

	virtual void soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext);

protected:
	virtual void _soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2);

	virtual void compute_gamma   (const mipp::vector<R> &sys, const mipp::vector<R> &par);
	virtual void compute_alpha   (                                                      );
	virtual void compute_beta    (                                                      );
	virtual void compute_ext     (const mipp::vector<R> &sys,       mipp::vector<R> &ext);
	virtual void compute_beta_ext(const mipp::vector<R> &sys,       mipp::vector<R> &ext);

	virtual void compute_gamma   (const R *sys, const R *par, const R *tail_sys, const R *tail_par);
	virtual void compute_ext_sys (const R *sys,                                        R *ext_sys );
	virtual void compute_ext_par (const R *par,                                        R *ext_par );
};
}
}

#include "Decoder_RSC_BCJR_seq_generic_std.hxx"

#endif /* DECODER_RSC_BCJR_STD_GENERIC_SEQ_HPP_ */
