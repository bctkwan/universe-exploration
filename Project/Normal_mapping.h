#ifndef  __NORMAL_MAPPING_H__
#define  __NORMAL_MAPPING_H__

#include "Dependencies\glm\glm.hpp"
#include <vector>

/*
* Source code of "Tutorial 13 : Normal Mapping" on "opengl-tutorial.org"
* http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
* Under the WTFPL Public License
*/

/*
* Source code obtained from:
* https://github.com/opengl-tutorials/ogl/blob/master/common/tangentspace.hpp
*/
void computeTangentBasis(
	// inputs
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals,
	// outputs
	std::vector<glm::vec3> & tangents,
	std::vector<glm::vec3> & bitangents
);

/*
* Source code obtained from:
* https://github.com/opengl-tutorials/ogl/blob/master/common/vboindexer.hpp
*/
void indexVBO_TBN(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,
	std::vector<glm::vec3> & in_tangents,
	std::vector<glm::vec3> & in_bitangents,

	std::vector<unsigned short> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::vec3> & out_tangents,
	std::vector<glm::vec3> & out_bitangents
);

#endif