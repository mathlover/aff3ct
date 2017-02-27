#include <iostream>
#include <iomanip>
#include <sstream>

#include "Tools/Display/bash_tools.h"

#include "Terminal_BFER.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Terminal_BFER<B,R>
::Terminal_BFER(const R& snr_s,
                const R& snr_b,
                const Monitor<B,R> &monitor,
                const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> &t_snr,
                const std::chrono::nanoseconds *d_decod_total)
: snr_s          (snr_s        ),
  snr_b          (snr_b        ),
  monitor        (monitor      ),
  t_snr          (t_snr        ),
  d_decod_total  (d_decod_total),
  real_time_state(0            )
{
}

template <typename B, typename R>
std::string Terminal_BFER<B,R>
::get_time_format(float secondes)
{
	auto ss = (int)secondes % 60;
	auto mm = ((int)(secondes / 60.f) % 60);
	auto hh = (int)(secondes / 3600.f);

	// TODO: this is not a C++ style code
	char time_format[256];
#ifdef _MSC_VER
	sprintf_s(time_format, 32, "%2.2dh%2.2d'%2.2d", hh, mm, ss);
#else
	sprintf(time_format, "%2.2dh%2.2d'%2.2d", hh, mm, ss);
#endif
	std::string time_format2(time_format);

	return time_format2;
}

template <typename B, typename R>
void Terminal_BFER<B,R>
::legend(std::ostream &stream)
{
#ifdef _WIN32
	if (this->d_decod_total == nullptr)
	{
		stream << "# " << bold("---------------------------------------------------------------------------||---------------------") << std::endl;
		stream << "# " << bold("         Bit Error Rate (BER) and Frame Error Rate (FER) depending         ||  Global throughput  ") << std::endl;
		stream << "# " << bold("                      on the Signal Noise Ratio (SNR)                      ||  and elapsed time   ") << std::endl;
		stream << "# " << bold("---------------------------------------------------------------------------||---------------------") << std::endl;
		stream << "# " << bold("-------|-------|-----------|-----------|-----------|-----------|-----------||----------|----------") << std::endl;
		stream << "# " << bold(" Es/N0 | Eb/N0 |       FRA |        BE |        FE |       BER |       FER || SIM_CTHR |    ET/RT ") << std::endl;
		stream << "# " << bold("  (dB) |  (dB) |           |           |           |           |           ||   (Mb/s) | (hhmmss) ") << std::endl;
		stream << "# " << bold("-------|-------|-----------|-----------|-----------|-----------|-----------||----------|----------") << std::endl;
	}
	else
	{
		stream << "# " << bold("---------------------------------------------------------------------------||---------------------------------||---------------------") << std::endl;
		stream << "# " << bold("         Bit Error Rate (BER) and Frame Error Rate (FER) depending         ||     Decoder throughput and      ||  Global throughput  ") << std::endl;
		stream << "# " << bold("                      on the Signal Noise Ratio (SNR)                      ||      latency (per thread)       ||  and elapsed time   ") << std::endl;
		stream << "# " << bold("---------------------------------------------------------------------------||---------------------------------||---------------------") << std::endl;
		stream << "# " << bold("-------|-------|-----------|-----------|-----------|-----------|-----------||----------|----------|-----------||----------|----------") << std::endl;
		stream << "# " << bold(" Es/N0 | Eb/N0 |       FRA |        BE |        FE |       BER |       FER ||     CTHR |    ITHR  |   LATENCY || SIM_CTHR |    ET/RT ") << std::endl;
		stream << "# " << bold("  (dB) |  (dB) |           |           |           |           |           ||   (Mb/s) |  (Mb/s)  |      (us) ||   (Mb/s) | (hhmmss) ") << std::endl;
		stream << "# " << bold("-------|-------|-----------|-----------|-----------|-----------|-----------||----------|----------|-----------||----------|----------") << std::endl;
	}
#else
	if (this->d_decod_total == nullptr)
	{
		stream << "# " << bold("----------------------------------------------------------------------||---------------------") << std::endl;
		stream << "# " << bold("      Bit Error Rate (BER) and Frame Error Rate (FER) depending       ||  Global throughput  ") << std::endl;
		stream << "# " << bold("                   on the Signal Noise Ratio (SNR)                    ||  and elapsed time   ") << std::endl;
		stream << "# " << bold("----------------------------------------------------------------------||---------------------") << std::endl;
		stream << "# " << bold("-------|-------|----------|----------|----------|----------|----------||----------|----------") << std::endl;
		stream << "# " << bold(" Es/N0 | Eb/N0 |      FRA |       BE |       FE |      BER |      FER || SIM_CTHR |    ET/RT ") << std::endl;
		stream << "# " << bold("  (dB) |  (dB) |          |          |          |          |          ||   (Mb/s) | (hhmmss) ") << std::endl;
		stream << "# " << bold("-------|-------|----------|----------|----------|----------|----------||----------|----------") << std::endl;
	}
	else
	{
		stream << "# " << bold("----------------------------------------------------------------------||--------------------------------||---------------------") << std::endl;
		stream << "# " << bold("      Bit Error Rate (BER) and Frame Error Rate (FER) depending       ||     Decoder throughput and     ||  Global throughput  ") << std::endl;
		stream << "# " << bold("                   on the Signal Noise Ratio (SNR)                    ||      latency (per thread)      ||  and elapsed time   ") << std::endl;
		stream << "# " << bold("----------------------------------------------------------------------||--------------------------------||---------------------") << std::endl;
		stream << "# " << bold("-------|-------|----------|----------|----------|----------|----------||----------|----------|----------||----------|----------") << std::endl;
		stream << "# " << bold(" Es/N0 | Eb/N0 |      FRA |       BE |       FE |      BER |      FER ||     CTHR |     ITHR |  LATENCY || SIM_CTHR |    ET/RT ") << std::endl;
		stream << "# " << bold("  (dB) |  (dB) |          |          |          |          |          ||   (Mb/s) |   (Mb/s) |     (us) ||   (Mb/s) | (hhmmss) ") << std::endl;
		stream << "# " << bold("-------|-------|----------|----------|----------|----------|----------||----------|----------|----------||----------|----------") << std::endl;
	}
#endif
}

template <typename B, typename R>
void Terminal_BFER<B,R>
::_report(std::ostream &stream)
{
	using namespace std::chrono;
	using namespace std;

	auto ber = 0.f, fer = 0.f;
	if (monitor.get_n_be() != 0 )
	{
		ber = (float)monitor.get_ber();
		fer = (float)monitor.get_fer();
	}
	else
	{
		ber = (1.f) / ((float)monitor.get_n_analyzed_fra()) / monitor.get_K();
		fer = (1.f) / ((float)monitor.get_n_analyzed_fra());
	}
	auto fra = monitor.get_n_analyzed_fra();
	auto be  = monitor.get_n_be();
	auto fe  = monitor.get_n_fe();

	auto dec_cthr = 0.f;
	auto dec_ithr = 0.f;
	auto lat      = 0.f;
	if (this->d_decod_total != nullptr)
	{
		auto decod_time_ms = (float)d_decod_total->count() * 0.000001f;
		auto total_time = d_decod_total->count();

		dec_cthr = ((float)monitor.get_N() * (float)monitor.get_n_analyzed_fra()) /
		           (total_time * 0.000000001f); // = bps
		dec_cthr /= 1000.f; // = kbps
		dec_cthr /= 1000.f; // = mbps

		dec_ithr = dec_cthr * ((float)monitor.get_K() / (float)monitor.get_N());

		lat = decod_time_ms * 1000.f;
		lat = (lat / (float) monitor.get_n_analyzed_fra()) * monitor.get_n_frames();
	}

	auto simu_time = (float)duration_cast<nanoseconds>(steady_clock::now() - t_snr).count() * 0.000000001f;
	auto simu_cthr = ((float)monitor.get_N() * (float)monitor.get_n_analyzed_fra()) /
		              simu_time ; // = bps
	simu_cthr /= 1000.f; // = kbps
	simu_cthr /= 1000.f; // = mbps

	if (Monitor<B,R>::is_interrupt()) stream << "\r";

#ifdef _WIN32
	stringstream str_ber, str_fer;
	str_ber << setprecision(2) << scientific << setw(9) << ber;
	str_fer << setprecision(2) << scientific << setw(9) << fer;

	unsigned long long l0 = 99999999;  // limit 0
	auto               l1 = 99999999;  // limit 1
	auto               l2 = 99999.99f; // limit 2
	stream << "  ";
	stream << setprecision(                 2) <<                            fixed  << setw(6) <<                           snr_s << bold(" | ");
	stream << setprecision(                 2) <<                            fixed  << setw(5) <<                           snr_b << bold(" | ");
	stream << setprecision((fra > l0) ? 2 : 0) << ((fra > l0) ? scientific : fixed) << setw(9) << ((fra > l0) ? (float)fra : fra) << bold(" | ");
	stream << setprecision(( be > l1) ? 2 : 0) << ((be  > l1) ? scientific : fixed) << setw(9) << (( be > l1) ? (float) be :  be) << bold(" | ");
	stream << setprecision(( fe > l1) ? 2 : 0) << ((fe  > l1) ? scientific : fixed) << setw(9) << (( fe > l1) ? (float) fe :  fe) << bold(" | ");
	stream <<                                                                                                       str_ber.str() << bold(" | ");
	stream <<                                                                                                       str_fer.str() << bold(" || ");
	if (this->d_decod_total != nullptr)
	{
		stream << setprecision(             2) <<                            fixed  << setw(8) <<                        dec_cthr << bold(" | ");
		stream << setprecision(             2) <<                            fixed  << setw(8) <<                        dec_ithr << bold(" | ");
		stream << setprecision(             2) << ((lat > l2) ? scientific : fixed) << setw(9) <<                             lat << bold(" || ");
	}
	stream << setprecision(                 2) <<                            fixed  << setw(8) <<                       simu_cthr;
#else
	stringstream str_ber, str_fer;
	str_ber << setprecision(2) << scientific << setw(8) << ber;
	str_fer << setprecision(2) << scientific << setw(8) << fer;

	unsigned long long l0 = 99999999;  // limit 0
	auto               l1 = 99999999;  // limit 1
	auto               l2 = 99999.99f; // limit 2
	stream << "  ";
	stream << setprecision(                 2) <<                            fixed  << setw(6) <<                           snr_s << bold(" | ");
	stream << setprecision(                 2) <<                            fixed  << setw(5) <<                           snr_b << bold(" | ");
	stream << setprecision((fra > l0) ? 2 : 0) << ((fra > l0) ? scientific : fixed) << setw(8) << ((fra > l0) ? (float)fra : fra) << bold(" | ");
	stream << setprecision(( be > l1) ? 2 : 0) << ((be  > l1) ? scientific : fixed) << setw(8) << (( be > l1) ? (float) be :  be) << bold(" | ");
	stream << setprecision(( fe > l1) ? 2 : 0) << ((fe  > l1) ? scientific : fixed) << setw(8) << (( fe > l1) ? (float) fe :  fe) << bold(" | ");
	stream <<                                                                                                       str_ber.str() << bold(" | ");
	stream <<                                                                                                       str_fer.str() << bold(" || ");
	if (this->d_decod_total != nullptr)
	{
		stream << setprecision(             2) <<                            fixed  << setw(8) <<                        dec_cthr << bold(" | ");
		stream << setprecision(             2) <<                            fixed  << setw(8) <<                        dec_ithr << bold(" | ");
		stream << setprecision(             2) << ((lat > l2) ? scientific : fixed) << setw(8) <<                             lat << bold(" || ");
	}
	stream << setprecision(                 2) <<                            fixed  << setw(8) <<                       simu_cthr;
#endif
}

template <typename B, typename R>
void Terminal_BFER<B,R>
::temp_report(std::ostream &stream)
{
	using namespace std::chrono;

	_report(stream);

	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto tr = et * ((float)monitor.get_fe_limit() / (float)monitor.get_n_fe()) - et;
	auto tr_format = get_time_format((monitor.get_n_fe() == 0) ? 0 : tr);

	stream << bold(" | ") << std::setprecision(0) << std::fixed << std::setw(8) << tr_format;

	stream << " ";
	switch (real_time_state)
	{
		case 0: stream << bold("*"); break;
		case 1: stream << bold("*"); break;
		case 2: stream << bold(" "); break;
		case 3: stream << bold(" "); break;
		default: break;
	}
	real_time_state = (real_time_state +1) % 4;
	stream << "\r";

	stream.flush();
}

template <typename B, typename R>
void Terminal_BFER<B,R>
::final_report(std::ostream &stream)
{
	using namespace std::chrono;

	this->_report(stream);

	auto et = duration_cast<milliseconds>(steady_clock::now() - t_snr).count() / 1000.f;
	auto et_format = get_time_format(et);

	stream << bold(" | ") << std::setprecision(0) << std::fixed << std::setw(8) << et_format;

	if (Monitor<B,R>::is_interrupt()) stream << " x" << std::endl;
	else                            stream << "  " << std::endl;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Terminal_BFER<B_8,R_8>;
template class aff3ct::tools::Terminal_BFER<B_16,R_16>;
template class aff3ct::tools::Terminal_BFER<B_32,R_32>;
template class aff3ct::tools::Terminal_BFER<B_64,R_64>;
#else
template class aff3ct::tools::Terminal_BFER<B,R>;
#endif
// ==================================================================================== explicit template instantiation
