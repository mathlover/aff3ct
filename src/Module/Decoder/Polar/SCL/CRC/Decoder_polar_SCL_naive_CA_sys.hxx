#include "Decoder_polar_SCL_naive_CA_sys.hpp"

namespace aff3ct
{
template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
Decoder_polar_SCL_naive_CA_sys<B,R,F,G>
::Decoder_polar_SCL_naive_CA_sys(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits,
                                 CRC<B>& crc, const int n_frames, const std::string name)
: Decoder_polar_SCL_naive_CA<B,R,F,G>(K, N, L, frozen_bits, crc, n_frames, name)
{
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive_CA_sys<B,R,F,G>
::select_best_path() 
{
	mipp::vector<B> U_test;
	std::set<int> active_paths_before_crc = this->active_paths;
	for (auto path : active_paths_before_crc)
	{
		U_test.clear();

		for (auto i = 0 ; i < this->N ; i++)
			if (!this->frozen_bits[i]) U_test.push_back(this->polar_trees[path]->get_root()->get_c()->s[i]);

		bool decode_result = this->crc.check(U_test, this->get_simd_inter_frame_level());
		if (!decode_result)
			this->active_paths.erase(path);
	}

	this->Decoder_polar_SCL_naive<B,R,F,G>::select_best_path();
}


template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive_CA_sys<B,R,F,G>
::store(mipp::vector<B>& V_K) const
{
	assert(V_K.size() >= (unsigned) this->K);

	auto k = 0;
	for (auto i = 0; i < this->N; i++)
		if (!this->frozen_bits[i])
			V_K[k++] = this->polar_trees[*this->active_paths.begin()]->get_root()->get_c()->s[i] ? 1 : 0;
}
}
