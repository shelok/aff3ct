#include <sstream>

#include "Tools/Noise/noise_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Tools/Codec/Codec.hpp"

namespace aff3ct
{
namespace tools
{

template <typename B, typename Q>
Codec<B,Q>
::Codec(const int K, const int N_cw, const int N, const int tail_length, const int n_frames)
: K(K), N_cw(N_cw), N(N), tail_length(tail_length), noise(nullptr)
{
	if (K <= 0)
	{
		std::stringstream message;
		message << "'K' has to be greater than 0 ('K' = " << K << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N_cw <= 0)
	{
		std::stringstream message;
		message << "'N_cw' has to be greater than 0 ('N_cw' = " << N_cw << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (K > N)
	{
		std::stringstream message;
		message << "'K' has to be smaller or equal to 'N' ('K' = " << K << ", 'N' = " << N << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N > N_cw)
	{
		std::stringstream message;
		message << "'N' has to be smaller or equal to 'N_cw' ('N' = " << N << ", 'N_cw' = " << N_cw << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename Q>
Codec<B,Q>
::~Codec()
{
	if (this->noise != nullptr)
		this->noise->unrecord_callback_changed(this->id_noise_changed_callback);
}

template <typename B, typename Q>
Interleaver_core<>& Codec<B,Q>
::get_interleaver()
{
	if (this->interleaver_core == nullptr)
	{
		std::stringstream message;
		message << "'interleaver_core' is NULL.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->interleaver_core;
}

template <typename B, typename Q>
module::Encoder<B>& Codec<B,Q>
::get_encoder()
{
	if (this->encoder == nullptr)
	{
		std::stringstream message;
		message << "'encoder' is NULL.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->encoder;
}

template <typename B, typename Q>
module::Puncturer<B,Q>& Codec<B,Q>
::get_puncturer()
{
	if (this->puncturer == nullptr)
	{
		std::stringstream message;
		message << "'puncturer' is NULL.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->puncturer;
}

template <typename B, typename Q>
module::Extractor<B,Q>& Codec<B,Q>
::get_extractor()
{
	if (this->extractor == nullptr)
	{
		std::stringstream message;
		message << "'extractor' is NULL.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->extractor;
}

template <typename B, typename Q>
const Noise<>& Codec<B,Q>
::get_noise() const
{
	if (this->noise == nullptr)
	{
		std::stringstream message;
		message << "'noise' should not be nullptr.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->noise;
}

template <typename B, typename Q>
void Codec<B,Q>
::set_noise(Noise<>& noise)
{
	if (&noise != this->noise)
	{
		if (this->noise != nullptr)
			this->noise->unrecord_callback_changed(this->id_noise_changed_callback);
		this->noise = &noise;
		this->id_noise_changed_callback = this->noise->record_callback_changed([this]()
		{
			if (this->noise->is_set())
				this->noise_changed();
		});
		this->check_noise();
		if (this->noise->is_set())
			this->noise_changed();
	}
}

template <typename B, typename Q>
void Codec<B,Q>
::noise_changed()
{
}

template <typename B, typename Q>
void Codec<B,Q>
::check_noise()
{
	if (this->noise == nullptr)
	{
		std::stringstream message;
		message << "'noise' should not be nullptr.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename Q>
void Codec<B,Q>
::set_interleaver(Interleaver_core<>* itl)
{
	this->set_interleaver(std::unique_ptr<Interleaver_core<>>(itl));
}

template <typename B, typename Q>
void Codec<B,Q>
::set_encoder(module::Encoder<B>* enc)
{
	this->set_encoder(std::unique_ptr<module::Encoder<B>>(enc));
}

template <typename B, typename Q>
void Codec<B,Q>
::set_puncturer(module::Puncturer<B,Q>* pct)
{
	this->set_puncturer(std::unique_ptr<module::Puncturer<B,Q>>(pct));
}

template <typename B, typename Q>
void Codec<B,Q>
::set_extractor(module::Extractor<B,Q>* ext)
{
	this->set_extractor(std::unique_ptr<module::Extractor<B,Q>>(ext));
}


template <typename B, typename Q>
void Codec<B,Q>
::set_interleaver(std::unique_ptr<Interleaver_core<>>&& itl)
{
	this->interleaver_core = std::move(itl);
	this->interleaver_bit.reset(factory::Interleaver::build<B>(*this->interleaver_core));
	this->interleaver_llr.reset(factory::Interleaver::build<Q>(*this->interleaver_core));
}

template <typename B, typename Q>
void Codec<B,Q>
::set_encoder(std::unique_ptr<module::Encoder<B>>&& enc)
{
	this->encoder = std::move(enc);
}

template <typename B, typename Q>
void Codec<B,Q>
::set_puncturer(std::unique_ptr<module::Puncturer<B,Q>>&& pct)
{
	this->puncturer = std::move(pct);
}

template <typename B, typename Q>
void Codec<B,Q>
::set_extractor(std::unique_ptr<module::Extractor<B,Q>>&& ext)
{
	this->extractor = std::move(ext);
}

template <typename B, typename Q>
const module::Interleaver<B>& Codec<B,Q>
::get_interleaver_bit()
{
	return *this->interleaver_bit;
}

template <typename B, typename Q>
const module::Interleaver<Q>& Codec<B,Q>
::get_interleaver_llr()
{
	return *this->interleaver_llr;
}

}
}
