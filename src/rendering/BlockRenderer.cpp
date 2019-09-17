#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "BlockRenderer.h"

#include "../renderModel/TextureArray.h"

#include <iostream>


#include "shaderPrograms/shaders/chunkCulling_vertex.glsl"
#include "shaderPrograms/shaders/chunkCulling_fragment.glsl"
#include "shaderPrograms/shaders/blocks_vertex.glsl"
#include "shaderPrograms/shaders/blocks_geometry.glsl"
#include "shaderPrograms/shaders/blocks_fragment.glsl"

namespace vc {
	const float BlockRenderer::CHUNK_CUBE_DATA[108] = {
			// oben
			0, Chunk::CHUNK_SIZE, 0, 0, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0, Chunk::CHUNK_SIZE,
			0, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0,

			// vorne
			0, 0, 0, 0, Chunk::CHUNK_SIZE, 0, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0, Chunk::CHUNK_SIZE, 0, 0, 0,
			0, 0,

			// unten
			0, 0, 0, Chunk::CHUNK_SIZE, 0, Chunk::CHUNK_SIZE, 0, 0, Chunk::CHUNK_SIZE, 0, 0, 0, Chunk::CHUNK_SIZE, 0, 0, Chunk::CHUNK_SIZE, 0,
			Chunk::CHUNK_SIZE,

			// hinten
			0, 0, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE,
			Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0, 0, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0, Chunk::CHUNK_SIZE,

			// links
			Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0, 0, Chunk::CHUNK_SIZE,
			Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0, 0,

			// rechts
			0, Chunk::CHUNK_SIZE, 0, 0, 0, 0, 0, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0, 0, 0, 0, 0,
			Chunk::CHUNK_SIZE,
	};

	BlockRenderer::BlockShader::BlockShader() :
			VertexGeometryFragmentShaderProgram(
				blocks_vertex,
				blocks_geometry,
				blocks_fragment,
				[this]() {
					this->createShaderVariable(0, "in_position");
					this->createShaderVariable(1, "in_texId_facing");
				}) {
		location_projectionViewMatrix = this->createUniformLocation("projectionViewMatrix");
		location_fogColor = this->createUniformLocation("fogColor");

	}

	void BlockRenderer::BlockShader::loadProjectionMatrix(glm::mat4x4 projectionViewMatrix) {
		using namespace std;
		loadMatrix4f(location_projectionViewMatrix, projectionViewMatrix);
	}


	void BlockRenderer::BlockShader::loadFogColor(glm::vec4 fogColor) {
		loadVector4f(location_fogColor, fogColor);
	}

	BlockRenderer::ChunkCullingShader::ChunkCullingShader() :
			VertexFragmentShaderProgram(
				chunkCulling_vertex,
				chunkCulling_fragment,
				[this]() {
					this->createShaderVariable(0, "vertexPosition");
				}) {

		location_projectionViewMatrix = this->createUniformLocation("projectionViewMatrix");
		location_chunkPosition = this->createUniformLocation("chunkPosition");
		location_heightInPercent = this->createUniformLocation("heightInPercent");
	}

	void BlockRenderer::ChunkCullingShader::loadProjectionMatrix(glm::mat4x4 projectionViewMatrix) {
		loadMatrix4f(location_projectionViewMatrix, projectionViewMatrix);
	}

	void BlockRenderer::ChunkCullingShader::loadChunkPosition(glm::ivec3 chunkPosition) {
		loadVector3f(location_chunkPosition, glm::vec3(chunkPosition.x, chunkPosition.y, chunkPosition.z));
	}

	void BlockRenderer::ChunkCullingShader::loadHeightInPercent(float heightInPercent) {
		loadFloat(location_heightInPercent, heightInPercent);
	}

	BlockRenderer::Query::Query(uint32_t queryId) :
			queryId(queryId)  {
	}

	uint32_t BlockRenderer::Query::getQueryId() const {
		return queryId;
	}

	Chunk* BlockRenderer::Query::getAssociatedChunk() {
		return p_associatedChunk;
	}

	void BlockRenderer::Query::setAssociatedChunk(Chunk* p_chunk) {
		this->p_associatedChunk = p_chunk;
	}

	bool BlockRenderer::Query::isRenderedAsVisible() const {
		return renderedAsVisible;
	}

	void BlockRenderer::Query::setRenderedAsVisible(bool renderedAsVisible) {
		this->renderedAsVisible = renderedAsVisible;
	}

	BlockRenderer::QueryList::QueryList(uint32_t size) {
		std::vector<uint32_t> queries(size);
		glGenQueries(size, &queries[0]);

		for(unsigned int counter = 0; counter < size; counter++) {
			Query* q = new Query(queries[counter]);
			allQueries.push_back(q);
			freeQueries.push_back(q);
		}

	}

	BlockRenderer::QueryList::~QueryList() {
		for(auto singleQuery : allQueries) {
			uint32_t q = singleQuery->getQueryId();
			uint32_t* p_q = &q;
			glDeleteQueries(1, p_q);
			delete singleQuery;
		}
	}

	BlockRenderer::Query* BlockRenderer::QueryList::getQuery() {
		Query* q = freeQueries.front();
		freeQueries.erase(freeQueries.begin());

		return q;
	}

	void BlockRenderer::QueryList::freeQuery(Query* q) {
		q->setAssociatedChunk(nullptr);
		freeQueries.push_back(q);
	}

	BlockRenderer::BlockRenderer(TextureArray blockTextureArray) :
			blockTextureArray(blockTextureArray),
			blockShader(),
			chunkCullingShader(),
			chunkCubeVao(std::vector<float>(CHUNK_CUBE_DATA, CHUNK_CUBE_DATA + sizeof(CHUNK_CUBE_DATA) / sizeof(CHUNK_CUBE_DATA[0])), 3),
			queryList(20 * 20 * 10) {


	}

	BlockRenderer::~BlockRenderer() {
	}

	void BlockRenderer::drawVisibleChunks(Level* p_level, std::vector<Chunk*>& chunkList, tl::optional<std::vector<Query*>&> queries) {
		// start shader
		blockShader.startProgram();

		// compute projectionViewMatrix
		glm::mat4x4 projectionViewMatrix = p_level->getPlayer().getFrustum().getProjectionMatrix() * p_level->getPlayer().computeViewMatrix();

		// load uniform variables
		blockShader.loadFogColor(p_level->getBackgroundColor());
		blockShader.loadProjectionMatrix(projectionViewMatrix);

		for(auto& singleChunk : chunkList) {
			// those chunks will be processed by drawInvisibleChunks()
			if(! singleChunk->wasLastTimeVisible()) continue;

			if(! queries.has_value()) {
				glBindVertexArray(singleChunk->getVao()->getVertexArrayObjectId());
				glDrawArrays(GL_POINTS, 0, GLsizei(singleChunk->getVao()->getIndiciesCount()));
			} else {
				Query* p_query = this->queryList.getQuery();
				queries->push_back(p_query);
				p_query->setAssociatedChunk(singleChunk);
				p_query->setRenderedAsVisible(true);

				glBeginQuery(GL_SAMPLES_PASSED, p_query->getQueryId());
				glBindVertexArray(singleChunk->getVao()->getVertexArrayObjectId());
				glDrawArrays(GL_POINTS, 0, GLsizei(singleChunk->getVao()->getIndiciesCount()));
				glEndQuery(GL_SAMPLES_PASSED);
			}
		}

		blockShader.stopProgram();
	}

	void BlockRenderer::drawInvisibleChunks(Level* p_level, std::vector<Chunk*>& chunkList, std::vector<Query*>& queries) {
		glDisable(GL_CULL_FACE);

		// the invisible chunks shouldn't be drawn
		glColorMask(false, false, false, false);
		glDepthMask(false);

		// start shader
		chunkCullingShader.startProgram();

		// compute projectionViewMatrix
		glm::mat4x4 projectionViewMatrix = p_level->getPlayer().getFrustum().getProjectionMatrix() * p_level->getPlayer().computeViewMatrix();

		// load uniform variables
		chunkCullingShader.loadProjectionMatrix(projectionViewMatrix);

		// bind VAO
		glBindVertexArray(chunkCubeVao.getVertexArrayObjectId());

		for(auto& singleChunk : chunkList) {
			// those chunks will be processes by drawVisibleChunks()
			if(singleChunk->wasLastTimeVisible()) continue;

			Query* p_query = this->queryList.getQuery();
			queries.push_back(p_query);
			p_query->setAssociatedChunk(singleChunk);
			p_query->setRenderedAsVisible(false);

			glm::ivec3 chunkCoords = singleChunk->getChunkCoordinates();
			chunkCoords *= static_cast<int>(Chunk::CHUNK_SIZE);

			chunkCullingShader.loadChunkPosition(chunkCoords);
			chunkCullingShader.loadHeightInPercent((singleChunk->getMaxHeight() + 1.0f) / Chunk::CHUNK_SIZE_FLOAT);

			glBeginQuery(GL_SAMPLES_PASSED, p_query->getQueryId());
			glDrawArrays(GL_TRIANGLES, 0, GLsizei(chunkCubeVao.getIndiciesCount()));
			glEndQuery(GL_SAMPLES_PASSED);
		}

		chunkCullingShader.stopProgram();
		glColorMask(true, true, true, true);
		glDepthMask(true);
		glEnable(GL_CULL_FACE);
	}

	void BlockRenderer::render(std::vector<Chunk*>& c, Level* p_level) {
		// collect the result of drawing the visible chunks in the last frame
		for(auto& q : queries_visibleChunks) {
			int32_t samplesPassed = 0;
			glGetQueryObjectiv(q->getQueryId(), GL_QUERY_RESULT, &samplesPassed);
			this->queryList.freeQuery(q);
		}


		// bind the needed texture array for rendering all the chunks
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, blockTextureArray.getTexId());


		queries_visibleChunks.clear();
		queries_invisibleChunks.clear();

		/*
		 * draws the chunks which have been visible in the last rendering call
		 *
		 * since drawVisibleChunks() will silently ignore chunks which haven't been visible then, we are allowed to pass
		 * the whole list with all chunks in the frustum
		 */
		drawVisibleChunks(p_level, c, queries_visibleChunks);


		/*
		 * draws the chunks which haven't been visible in the last rendering call
		 *
		 * since drawInvisibleChunks() will silently ignore chunks which haven't been visible then, we are allowed to pass
		 * the whole list with all chunks in the frustum
		 */
		drawInvisibleChunks(p_level, c, queries_invisibleChunks);


		// we have to check all invisible chunks if they are still invisible
		// if not, we have to render them additionally now
		std::vector<Chunk*> toRender;
		for(auto& q : queries_invisibleChunks) {
			bool wasVisible = q->getAssociatedChunk()->wasLastTimeVisible();

			int32_t samplesPassed = 0;
			glGetQueryObjectiv(q->getQueryId(), GL_QUERY_RESULT, &samplesPassed);
			q->getAssociatedChunk()->setLastTimeVisible(samplesPassed != 0);

			if(! wasVisible && samplesPassed != 0) toRender.push_back(q->getAssociatedChunk());

			this->queryList.freeQuery(q);
		}

		/**
		 * now we can draw the chunks determined to rendered additionally because they're visible. We don't need a second
		 * check wheter they're visible or not, so we can pass null as query list.
		 */
		drawVisibleChunks(p_level, toRender, tl::nullopt);
	}
}


