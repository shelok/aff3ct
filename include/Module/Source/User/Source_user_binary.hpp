#ifndef SOURCE_USER_BINARY_HPP_
#define SOURCE_USER_BINARY_HPP_

#include <string>
#include <vector>
#include <fstream>
#include <memory>

#include "Module/Source/Source.hpp"

namespace aff3ct
{
namespace module
{
template <typename B>
class Source_user_binary : public Source<B>
{
private:
	std::ifstream source_file;
	const bool auto_reset;
	bool over;
	size_t n_left;
	std::vector<char> memblk;
	std::vector<B> left_bits; // to store bits that are left by last call (n_left & n_completing)

public:
	Source_user_binary(const int K, std::string filename, const bool auto_reset = true, const int n_frames = 1);
	virtual ~Source_user_binary() = default;

	virtual bool is_over() const;

	virtual void reset();

protected:
	void _generate(B *U_K, const int frame_id);
};
}
}

#endif /* SOURCE_USER_BINARY_HPP_ */
