#ifndef SC_SISO_HPP_
#define SC_SISO_HPP_

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
template <typename R>
class SC_SISO;

template <typename R = float>
class SC_SISO_module : public sc_core::sc_module
{
	SC_HAS_PROCESS(SC_SISO_module);

public:
	tlm_utils::simple_target_socket   <SC_SISO_module> s_in;
	tlm_utils::simple_initiator_socket<SC_SISO_module> s_out;

private:
	SC_SISO<R> &siso;
	mipp::vector<R> Y_N1, Y_N2;

public:
	SC_SISO_module(SC_SISO<R> &siso, const sc_core::sc_module_name name = "SC_SISO_module")
	: sc_module(name), s_in ("s_in"), s_out("s_out"),
	  siso(siso),
	  Y_N1(siso.N_siso * siso.n_frames),
	  Y_N2(siso.N_siso * siso.n_frames)
	{
		s_in.register_b_transport(this, &SC_SISO_module::b_transport);
	}

	void resize_buffers()
	{
		if ((int)Y_N1.size() != siso.N_siso * siso.n_frames) Y_N1.resize(siso.N_siso * siso.n_frames);
		if ((int)Y_N2.size() != siso.N_siso * siso.n_frames) Y_N2.resize(siso.N_siso * siso.n_frames);
	}

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		assert((trans.get_data_length() / sizeof(R)) == Y_N1.size());

		const R* buffer_in = (R*)trans.get_data_ptr();
		std::copy(buffer_in, buffer_in + Y_N1.size(), Y_N1.begin());

		siso.soft_decode(Y_N1, Y_N2);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)Y_N2.data());
		payload.set_data_length(Y_N2.size() * sizeof(R));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		s_out->b_transport(payload, zero_time);
	}
};

template <typename R>
class SC_SISO : public SISO_i<R>
{
	friend SC_SISO_module<R>;

public:
	SC_SISO_module<R> *module_siso;

public:
	SC_SISO(const int K, const int N, const int n_frames, const int simd_inter_frame_level = 1,
	        const std::string name = "SC_SISO")
	: SISO_i<R>(K, N, n_frames, simd_inter_frame_level, name), module_siso(nullptr) {}

	virtual ~SC_SISO() { if (module_siso != nullptr) { delete module_siso; module_siso = nullptr; } }

	void create_sc_module_siso()
	{
		this->module_siso = new SC_SISO_module<R>(*this, this->name.c_str());
	}
};

template <typename R>
using SISO = SC_SISO<R>;
}
}
#else
#include "SPU_SISO.hpp"
#endif

#endif /* SC_SISO_HPP_ */
