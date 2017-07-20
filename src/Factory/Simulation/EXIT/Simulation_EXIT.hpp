#ifndef FACTORY_SIMULATION_EXIT_HPP_
#define FACTORY_SIMULATION_EXIT_HPP_

#include <string>
#include <thread>

#include "Factory/Module/Channel.hpp"
#include "Factory/Module/Code/Decoder.hpp"
#include "Factory/Module/Code/Encoder.hpp"
#include "Factory/Module/Code/Puncturer.hpp"
#include "Factory/Module/Modem.hpp"
#include "Factory/Module/Monitor.hpp"
#include "Factory/Module/Quantizer.hpp"
#include "Factory/Module/Source.hpp"
#include "Factory/Tools/Display/Terminal/EXIT/Terminal_EXIT.hpp"

#include "Tools/Arguments_reader.hpp"

#include "../Simulation.hpp"

namespace aff3ct
{
namespace factory
{
struct Simulation_EXIT : Simulation
{
	struct parameters : Simulation::parameters
	{
		parameters()
		: Simulation::parameters(),
		  src(new Source       ::parameters()),
		  enc(new Encoder      ::parameters()),
		  pct(new Puncturer    ::parameters()),
		  mdm(new Modem        ::parameters()),
		  chn(new Channel      ::parameters()),
		  dec(new Decoder      ::parameters()),
		  ter(new Terminal_EXIT::parameters())
		{
		}

		virtual ~parameters()
		{
			if (this->src != nullptr) { delete this->src; this->src = nullptr; }
			if (this->enc != nullptr) { delete this->enc; this->enc = nullptr; }
			if (this->pct != nullptr) { delete this->pct; this->pct = nullptr; }
			if (this->mdm != nullptr) { delete this->mdm; this->mdm = nullptr; }
			if (this->chn != nullptr) { delete this->chn; this->chn = nullptr; }
			if (this->dec != nullptr) { delete this->dec; this->dec = nullptr; }
			if (this->ter != nullptr) { delete this->ter; this->ter = nullptr; }
		}

		std::string snr_type   = "ES";
		float       sig_a_min  = 0.0f;
		float       sig_a_max  = 5.0f;
		float       sig_a_step = 0.5f;
		bool        debug      = false;

		Source       ::parameters *src = nullptr;
		Encoder      ::parameters *enc = nullptr;
		Puncturer    ::parameters *pct = nullptr;
		Modem        ::parameters *mdm = nullptr;
		Channel      ::parameters *chn = nullptr;
		Decoder      ::parameters *dec = nullptr;
		Terminal_EXIT::parameters *ter = nullptr;
	};

	static void build_args(arg_map &req_args, arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, parameters &params);
	static void group_args(arg_grp& ar);

	static void header(params_list& head_sim, const parameters& params);
};
}
}

#endif /* FACTORY_SIMULATION_EXIT_HPP_ */