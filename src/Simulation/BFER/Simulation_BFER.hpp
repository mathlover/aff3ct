#ifndef SIMULATION_BFER_I_HPP_
#define SIMULATION_BFER_I_HPP_

#include <chrono>
#include <vector>
#include <random>

#include "Tools/params.h"
#include "Tools/Threads/Barrier.hpp"

#include "Module/Source/Source.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Puncturer/Puncturer.hpp"
#include "Module/Modulator/Modulator.hpp"
#include "Module/Channel/Channel.hpp"
#include "Module/Quantizer/Quantizer.hpp"
#include "Module/Coset/Coset.hpp"
#include "Module/Decoder/Decoder.hpp"
#include "Module/Monitor/Monitor.hpp"

#include "../Simulation.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class Simulation_BFER_i : public Simulation
{
protected:
	// simulation parameters
	const tools::parameters &params;

	// a barrier to synchronize the threads
	tools::Barrier barrier;

	// code specifications
	float snr;
	float snr_s;
	float snr_b;
	float code_rate;
	float sigma;

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_snr;

	// a vector of random generator to generate the seeds
	std::vector<std::mt19937> rd_engine_seed;

	// communication chain
	std::vector<module::Source   <B    >*> source;
	std::vector<module::CRC      <B    >*> crc;
	std::vector<module::Encoder  <B    >*> encoder;
	std::vector<module::Puncturer<B,Q  >*> puncturer;
	std::vector<module::Modulator<B,R,R>*> modulator;
	std::vector<module::Channel  <R    >*> channel;
	std::vector<module::Quantizer<R,Q  >*> quantizer;
	std::vector<module::Coset    <B,Q  >*> coset_real;
	std::vector<module::Decoder  <B,Q  >*> decoder;
	std::vector<module::Coset    <B,B  >*> coset_bit;
	std::vector<module::Monitor  <B,R  >*> monitor;

public:
	Simulation_BFER_i(const tools::parameters& params);
	virtual ~Simulation_BFER_i();
	void launch();

protected:
	static void build_communication_chain(Simulation_BFER_i<B,R,Q> *simu, const int tid = 0);

	virtual void launch_precompute ();
	virtual void snr_precompute    ();
	virtual void _launch           () = 0;
	virtual void snr_postcompute   ();
	virtual void release_objects   ();
	virtual void launch_postcompute();

	virtual module::Source   <B    >* build_source    (                const int tid = 0);
	virtual module::CRC      <B    >* build_crc       (                const int tid = 0);
	virtual module::Encoder  <B    >* build_encoder   (                const int tid = 0);
	virtual module::Puncturer<B,Q  >* build_puncturer (                const int tid = 0);
	virtual module::Modulator<B,R,R>* build_modulator (                const int tid = 0);
	virtual module::Channel  <R    >* build_channel   (const int size, const int tid = 0);
	virtual module::Quantizer<R,Q  >* build_quantizer (const int size, const int tid = 0);
	virtual module::Coset    <B,Q  >* build_coset_real(                const int tid = 0);
	virtual module::Decoder  <B,Q  >* build_decoder   (                const int tid = 0) = 0;
	virtual module::Coset    <B,B  >* build_coset_bit (                const int tid = 0);
	virtual module::Monitor  <B,R  >* build_monitor   (                const int tid = 0);
};
}
}

#endif /* SIMULATION_BFER_I_HPP_ */
