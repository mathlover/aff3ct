#ifndef DECODER_TURBO_HPP_
#define DECODER_TURBO_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor.hpp"

#include "../../Interleaver/Interleaver.hpp"

#include "../Decoder.hpp"
#include "../SISO.hpp"

namespace aff3ct
{
template <typename B, typename R>
class Decoder_turbo : public Decoder<B,R>
{
protected:
	const int  n_ite; // number of iterations
	const bool buffered_encoding;

	const Interleaver<short> &pi;
	SISO<R> &siso_n;
	SISO<R> &siso_i;

	Scaling_factor<R>& scaling_factor;

	mipp::vector<R> l_sn;  // systematic LLRs                  in the natural     domain
	mipp::vector<R> l_si;  // systematic LLRs                  in the interleaved domain
	mipp::vector<R> l_sen; // systematic LLRs + extrinsic LLRs in the natural     domain
	mipp::vector<R> l_sei; // systematic LLRs + extrinsic LLRs in the interleaved domain
	mipp::vector<R> l_pn;  // parity     LLRs                  in the natural     domain
	mipp::vector<R> l_pi;  // parity     LLRs                  in the interleaved domain
	mipp::vector<R> l_e1n; // extrinsic  LLRs                  in the natural     domain
	mipp::vector<R> l_e2n; // extrinsic  LLRs                  in the natural     domain
	mipp::vector<R> l_e1i; // extrinsic  LLRs                  in the interleaved domain
	mipp::vector<R> l_e2i; // extrinsic  LLRs                  in the interleaved domain
	mipp::vector<B> s;     // bit decision

public:
	Decoder_turbo(const int& K,
	              const int& N_without_tb,
	              const int& n_ite,
	              const Interleaver<short> &pi,
	              SISO<R> &siso_n,
	              SISO<R> &siso_i,
	              Scaling_factor<R> &scaling_factor,
	              const bool buffered_encoding = true,
	              const std::string name = "Decoder_turbo");
	virtual ~Decoder_turbo();

protected:
	virtual void load (const mipp::vector<R>& Y_N);
	virtual void store(      mipp::vector<B>& V_K) const;

private:
	void buffered_load(const mipp::vector<R>& Y_N);
	void standard_load(const mipp::vector<R>& Y_N);
};
}

#endif /* DECODER_TURBO_HPP_ */
