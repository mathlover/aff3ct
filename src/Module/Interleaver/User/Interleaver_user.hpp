#ifndef INTERLEAVER_USER_HPP
#define INTERLEAVER_USER_HPP

#include <fstream>

#include "Tools/Display/bash_tools.h"

#include "../Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename T>
class Interleaver_user : public Interleaver<T>
{
private:
	const std::string filename;

public:
	Interleaver_user(int size, const std::string filename, const std::string name = "Interleaver_user")
	: Interleaver<T>(size, 1, name), filename(filename) { gen_lookup_tables(); }

protected:
	void gen_lookup_tables()
	{
		std::ifstream file(filename.c_str(), std::ios::in);

		if (file.is_open())
		{
			T val;

			file >> val;
			if (val == (T)this->pi.size())
			{
				for (auto i = 0; i < (int)this->pi.size(); i++)
				{
					file >> val;
					if (val >= 0 && val < (T)this->pi.size())
					{
						auto found = false;
						auto j = 0;
						while(j < i && !found)
						{
							if (this->pi[j] == val)
								found = true;
							j++;
						}

						if (!found)
							this->pi[i] = (T)val;
						else
						{
							std::cerr << tools::bold_red("(EE) The interleaver value is wrong, it already")
							          << tools::bold_red(" exists elsewhere (read: ")
							          << tools::bold_red(std::to_string(val))
							          << tools::bold_red("), exiting.")
							          << std::endl;
							file.close();
							std::exit(-1);
						}
					}
					else
					{
						std::cerr << tools::bold_red("(EE) The interleaver value is wrong (read: ")
						          << tools::bold_red(std::to_string(val)) << tools::bold_red(", expected: < ")
						          << tools::bold_red(std::to_string(this->pi.size()))
						          << tools::bold_red("), exiting.")
						          << std::endl;
						file.close();
						std::exit(-1);
					}
				}

				for (auto i = 0; i < (int)this->pi_inv.size(); i++)
					this->pi_inv[this->pi[i]] = i;
			}
			else
			{
				std::cerr << tools::bold_red("(EE) The interleaver size is wrong (read: ")
				          << tools::bold_red(std::to_string(val))
				          << tools::bold_red(", expected: ") << tools::bold_red(std::to_string(this->pi.size()))
				          << tools::bold_red("), exiting.") << std::endl;
				file.close();
				std::exit(-1);
			}

			file.close();
		}
		else
		{
			std::cerr << tools::bold_red("(EE) Can't open \"")
			          << tools::bold_red(filename)
			          << tools::bold_red("\" file, exiting.")
			          << std::endl;
			std::exit(-1);
		}
	}
};
}
}

#endif	/* INTERLEAVER_USER_HPP */

