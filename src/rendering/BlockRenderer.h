#pragma once

#include "shaderPrograms/VertexGeometryFragmentShaderProgram.h"
#include "shaderPrograms/VertexFragmentShaderProgram.h"
#include <vector>
#include "../model/Chunk.h"
#include "../renderModel/GeneralVertexArrayObject.h"
#include "../renderModel/TextureArray.h"


namespace vc {
	class BlockRenderer {
		// ----------------------------------------------------------------------
		// -----------------------------CHILD-CLASS------------------------------
		// ----------------------------------------------------------------------
		private:
			class BlockShader : public VertexGeometryFragmentShaderProgram {
				// ----------------------------------------------------------------------
				// --------------------------------FIELDS--------------------------------
				// ----------------------------------------------------------------------
				public:
					int location_projectionViewMatrix = 0;
					int location_fogColor = 0;

				// ----------------------------------------------------------------------
				// -----------------------------CONSTRUCTORS-----------------------------
				// ----------------------------------------------------------------------
				public:
					BlockShader();

				// ----------------------------------------------------------------------
				// -------------------------------METHODS--------------------------------
				// ----------------------------------------------------------------------
				public:
					void loadProjectionMatrix(glm::mat4x4 projectionViewMatrix);
					void loadFogColor(glm::vec4 fogColor);
			};


			class ChunkCullingShader : public VertexFragmentShaderProgram {
				// ----------------------------------------------------------------------
				// --------------------------------FIELDS--------------------------------
				// ----------------------------------------------------------------------
				private:
					int location_projectionViewMatrix = 0;
					int location_chunkPosition = 0;
					int location_heightInPercent = 0;

				// ----------------------------------------------------------------------
				// -----------------------------CONSTRUCTORS-----------------------------
				// ----------------------------------------------------------------------
				public:
					ChunkCullingShader();

				// ----------------------------------------------------------------------
				// -------------------------------METHODS--------------------------------
				// ----------------------------------------------------------------------
				public:
					void loadProjectionMatrix(glm::mat4x4 projectionViewMatrix);
					void loadChunkPosition(glm::ivec3 cameraPosition);
					void loadHeightInPercent(float heightInPercent);
			};


			class Query {
				// ----------------------------------------------------------------------
				// --------------------------------FIELDS--------------------------------
				// ----------------------------------------------------------------------
				private:
					uint32_t const queryId;
					Chunk* p_associatedChunk = nullptr;
					bool renderedAsVisible = false;

				// ----------------------------------------------------------------------
				// -----------------------------CONSTRUCTORS-----------------------------
				// ----------------------------------------------------------------------
				public:
					Query(uint32_t queryId);
					Query(const Query&) = delete;


				// ---------------------------------------------------------------------
				// -------------------------------METHODS--------------------------------
				// ----------------------------------------------------------------------
				public:
					uint32_t getQueryId() const;
					Chunk* getAssociatedChunk();
					void setAssociatedChunk(Chunk* p_chunk);
					bool isRenderedAsVisible() const;
					void setRenderedAsVisible(bool renderedAsVisible);
			};


			class QueryList {
				// ----------------------------------------------------------------------
				// --------------------------------FIELDS--------------------------------
				// ----------------------------------------------------------------------
				private:
					std::vector<Query*> allQueries;
					std::vector<Query*> freeQueries;

				// ----------------------------------------------------------------------
				// -----------------------------CONSTRUCTORS-----------------------------
				// ----------------------------------------------------------------------
				public:
					QueryList(uint32_t size);
					QueryList(const QueryList&) = delete;
					~QueryList();

				// ----------------------------------------------------------------------
				// -------------------------------METHODS--------------------------------
				// ----------------------------------------------------------------------
				public:
					Query* getQuery();
					void freeQuery(Query* q);
			};


		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static const float CHUNK_CUBE_DATA[108];

	
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			TextureArray blockTextureArray;

			BlockShader blockShader;
			ChunkCullingShader chunkCullingShader;
			GeneralVertexArrayObject chunkCubeVao;

			QueryList queryList;

			std::vector<Query*> queries_visibleChunks;
			std::vector<Query*> queries_invisibleChunks;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			BlockRenderer(TextureArray blockTextureArray);
			BlockRenderer(const BlockRenderer&) = delete;
			~BlockRenderer();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void drawVisibleChunks(Level* p_level, std::vector<Chunk*>& chunkList, tl::optional<std::vector<Query*>&> queries);
			void drawInvisibleChunks(Level* p_level, std::vector<Chunk*>& chunkList, std::vector<Query*>& queries);

		public:
			void render(std::vector<Chunk*>& c, Level* level);
	};
}
