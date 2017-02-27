#ifndef DECODER_POLAR_SC_NAIVE_
#define DECODER_POLAR_SC_NAIVE_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Algo/Tree/Binary_tree.hpp"

#include "../../Decoder.hpp"
#include "../decoder_polar_functions.h"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Contents_SC
{
public:
	mipp::vector<R> lambda;
	mipp::vector<B> s;
	B               is_frozen_bit;

	Contents_SC(int size) : lambda(size), s(size), is_frozen_bit(0) {}
	virtual ~Contents_SC() {}
};

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_h<B,R> H>
class Decoder_polar_SC_naive : public Decoder<B,R>
{
protected:
	const int m; // graph depth

	const mipp::vector<B> &frozen_bits;
	tools::Binary_tree<Contents_SC<B,R>> polar_tree;

public:
	Decoder_polar_SC_naive(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1,
 	                       const std::string name = "Decoder_polar_SC_naive");
	virtual ~Decoder_polar_SC_naive();

protected:
	        void  load       (const mipp::vector<R>& Y_N);
	        void _hard_decode(                          );
	virtual void  store      (      mipp::vector<B>& V_K) const;

private:
	void recursive_allocate_nodes_contents  (      tools::Binary_node<Contents_SC<B,R>>* node_curr, const int vector_size                     );
	void recursive_initialize_frozen_bits   (const tools::Binary_node<Contents_SC<B,R>>* node_curr, const mipp::vector<B>& frozen_bits        );
	void recursive_decode                   (const tools::Binary_node<Contents_SC<B,R>>* node_curr                                            );
	void recursive_store                    (const tools::Binary_node<Contents_SC<B,R>>* node_curr,       mipp::vector<B>& V_K,         int &k) const;
	void recursive_deallocate_nodes_contents(      tools::Binary_node<Contents_SC<B,R>>* node_curr                                            );
};
}
}

#include "Decoder_polar_SC_naive.hxx"

#endif /* DECODER_POLAR_SC_NAIVE_ */
