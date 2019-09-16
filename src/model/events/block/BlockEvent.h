#pragma once
#include "../../AbstractBlock.h"
#include "../../AbstractPlayer.h"

namespace vc {
	namespace model {

		class BlockEvent {

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			protected:
				AbstractBlock* const p_block;
				AbstractPlayer* const p_player;


			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			protected:
				BlockEvent(AbstractBlock* const p_block, AbstractPlayer* const p_player);

			public:
				BlockEvent(const BlockEvent&) = delete;


			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				AbstractBlock* const getBlock() const;
				AbstractPlayer* const getPlayer() const;
		};	
	}
}
