#include "meshRenderer.h"

MeshRenderer::MeshRenderer(const std::string& modelPath, const std::string& texPath, const ShaderProgram& _shaderProgram) {
	loadModel(modelPath, model);
	texture = Texture(texPath.c_str(), 0);

	/*
	ebo.setData((unsigned int*)&indicies[0], indicies.size() * sizeof(indicies[0]), GL_STATIC_DRAW);
	vbo.setData((float*)&vertices[0], vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);

	vao.bind();
	ebo.bind();

	vao.attachVBO(vbo, 0, 3, sizeof(Vertex), offsetof(Vertex, pos));
	vao.attachVBO(vbo, 1, 3, sizeof(Vertex), offsetof(Vertex, normal));
	vao.attachVBO(vbo, 2, 2, sizeof(Vertex), offsetof(Vertex, texCoord));

	vao.unbind();
	vbo.unbind();
	ebo.unbind();
	*/

	shaderProgram = _shaderProgram;
}


void MeshRenderer::render() {
	texture.bind();
	shaderProgram.bind();
	/*
	model.vao.bind();
	glDrawElements(GL_TRIANGLES, model.indicies.size(), GL_UNSIGNED_INT, 0);
	model.vao.unbind();
	*/
	model.render();
	shaderProgram.unbind();
	texture.unbind();
}
