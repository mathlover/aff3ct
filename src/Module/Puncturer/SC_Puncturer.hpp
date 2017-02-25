#ifndef SC_PUNCTURER_HPP_
#define SC_PUNCTURER_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <cassert>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "Tools/Perf/MIPP/mipp.h"

namespace aff3ct
{
namespace module
{
template <typename B, typename Q>
class SC_Puncturer;

template <typename B, typename Q>
class SC_Puncturer_module_puncturer : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Puncturer_module_puncturer);

public:
	tlm_utils::simple_target_socket   <SC_Puncturer_module_puncturer> s_in;
	tlm_utils::simple_initiator_socket<SC_Puncturer_module_puncturer> s_out;

private:
	SC_Puncturer<B,Q> &puncturer;
	mipp::vector<B> X_N1, X_N2;

public:
	SC_Puncturer_module_puncturer(SC_Puncturer<B,Q> &puncturer, 
	                              const sc_core::sc_module_name name = "SC_Puncturer_module_puncturer")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  puncturer(puncturer),
	  X_N1(puncturer.N_code * puncturer.n_frames),
	  X_N2(puncturer.N      * puncturer.n_frames)
	{
		s_in.register_b_transport(this, &SC_Puncturer_module_puncturer::b_transport);
	}

	void resize_buffers()
	{
		if ((int)X_N1.size() != puncturer.N_code * puncturer.n_frames) X_N1.resize(puncturer.N_code * puncturer.n_frames);
		if ((int)X_N2.size() != puncturer.N      * puncturer.n_frames) X_N2.resize(puncturer.N      * puncturer.n_frames);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(B)) == X_N1.size());

		const B* buffer_in = (B*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + X_N1.size(), X_N1.begin());

		puncturer.puncture(X_N1, X_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)X_N2.data());
		payload.set_data_length(X_N2.size() * sizeof(B));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename Q>
class SC_Puncturer_module_depuncturer : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_Puncturer_module_depuncturer);

public:
	tlm_utils::simple_target_socket   <SC_Puncturer_module_depuncturer> s_in;
	tlm_utils::simple_initiator_socket<SC_Puncturer_module_depuncturer> s_out;

private:
	SC_Puncturer<B,Q> &puncturer;
	mipp::vector<Q> Y_N1, Y_N2;

public:
	SC_Puncturer_module_depuncturer(SC_Puncturer<B,Q> &puncturer,
	                                const sc_core::sc_module_name name = "SC_Puncturer_module_depuncturer")
	: sc_module(name), s_in("s_in"), s_out("s_out"),
	  puncturer(puncturer),
	  Y_N1(puncturer.N      * puncturer.n_frames),
	  Y_N2(puncturer.N_code * puncturer.n_frames)
	{
		s_in.register_b_transport(this, &SC_Puncturer_module_depuncturer::b_transport);
	}

	void resize_buffers()
	{
		if ((int)Y_N1.size() != puncturer.N      * puncturer.n_frames) Y_N1.resize(puncturer.N      * puncturer.n_frames);
		if ((int)Y_N2.size() != puncturer.N_code * puncturer.n_frames) Y_N2.resize(puncturer.N_code * puncturer.n_frames);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(Q)) == (int)Y_N1.size());

		const Q* buffer_in = (Q*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + Y_N1.size(), Y_N1.begin());

		puncturer.depuncture(Y_N1, Y_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(Q));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename B, typename Q>
class SC_Puncturer : public Puncturer_i<B,Q>
{
	friend SC_Puncturer_module_puncturer  <B,Q>;
	friend SC_Puncturer_module_depuncturer<B,Q>;

public:
	SC_Puncturer_module_puncturer  <B,Q> *module_punct;
	SC_Puncturer_module_depuncturer<B,Q> *module_depunct;

public:
	SC_Puncturer(const int K, const int N, const int N_code, const int n_frames = 1, 
	             const std::string name = "SC_Puncturer")
	: Puncturer_i<B,Q>(K, N, N_code, n_frames, name),
	  module_punct(nullptr), module_depunct(nullptr) {}

	virtual ~SC_Puncturer() 
	{ 
		if (module_punct   != nullptr) { delete module_punct;   module_punct   = nullptr; }
		if (module_depunct != nullptr) { delete module_depunct; module_depunct = nullptr; }
	}

	virtual void set_n_frames(const int n_frames)
	{
		Puncturer_i<B,Q>::set_n_frames(n_frames);

		if (module_punct   != nullptr) module_punct  ->resize_buffers();
		if (module_depunct != nullptr) module_depunct->resize_buffers();
	}

	void create_sc_module_puncturer()
	{
		const std::string new_name = this->name + "_punct";
		this->module_punct = new SC_Puncturer_module_puncturer<B,Q>(*this, new_name.c_str());
	}

	void create_sc_module_depuncturer()
	{
		const std::string new_name = this->name + "_depunct";
		this->module_depunct = new SC_Puncturer_module_depuncturer<B,Q>(*this, new_name.c_str());
	}
};

template <typename B, typename Q>
using Puncturer = SC_Puncturer<B,Q>;
}
}
#else
#include "SPU_Puncturer.hpp"
#endif

#endif /* SC_PUNCTURER_HPP_ */
