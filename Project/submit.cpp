#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glui\glui.h"
#include "Dependencies\irrKlang\irrKlang.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "shader_install.h"
#include "Loading_func.h"
#include "Bind_Data.h"

using namespace irrklang;
using namespace std;
using glm::vec3;
using glm::mat4;
// ============================= //
// window size
int Win_w, Win_h;
float camera_fov = 45.0;

int  mainWindowID;
GLint programID;
GLint Lightbox_programID;
GLint Skybox_programID;
GLint Rock_programID;

GLuint TextureEarth[2];
GLuint TextureApple;
GLuint TextureGlass[2];
GLuint TextureHelicopter;
GLuint TextureSkybox;
GLuint TextureRock;
GLuint TextureStar;

extern GLuint earthVao;
extern GLuint helicopterVao;
extern GLuint skyboxVao;
extern GLuint rockVao;
extern GLuint starVao;
extern int drawEarthSize;
extern int drawHelicopterSize;
extern int drawRockSize;
extern int drawStarSize;
// view matrix
glm::mat4 common_viewM;
glm::mat4 common_projection;

glm::mat4 helicopterModel = glm::mat4(0.0f);
glm::mat4 glassModel;
extern std::vector<glm::vec3> earthVertices;
extern std::vector<glm::vec3> helicopterVertices;

glm::vec3 lightbox1_pos;
glm::vec3 lightbox1_color = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightbox2_pos;
glm::vec3 lightbox2_color = glm::vec3(1.0f, 1.0f, 1.0f);

float earth_innRot_Degree = 0.0f;
float apple_innRot_Degree = 0.0f;
float apple_extRot_Degree = 0.0f;
float glass_innRot_Degree = 0.0f;
float lightbox_extRot_Degree = 0.0f;
float helicopter_extRot_Degree = 0.0f;

float helicopter_init_radius = 10.0f;
float helicopter_init_speed = 0.5;
float helicopter_radius = helicopter_init_radius;
float helicopter_speed = helicopter_init_speed;

// ============================= //
const float M_PI = 3.14159265;
float initRadius = 30.0f;
float radius = initRadius;
float initViewHorizontal = -90.0f;
float initViewVertical = -90.0f;

float cameraX = 0;
float cameraY = 0;
float cameraZ = radius;
float cameraDeltaX = 0;
float cameraDeltaY = 0;
float cameraTransX = 0;
float cameraTransY = 0;
float cameraTransZ = 0;

float viewRotateDegree[3] = { 0.0f, 0.0f, 0.0f };

GLUI *glui;
ISoundEngine* se;

int mouseLastX = -1;
int mouseLastY = -1;
float mouseSensitivity = 0.1;
int mouseControl = 0;

float a_brightness = 0.2f;
float d_brightness = 0.6f;
float s_brightness = 0.5f;

unsigned int rockAmount = 5001;
unsigned int maxStarAmount = 20;
unsigned int starAmount = 0;
int starLastUsed = -1;
glm::mat4 *starModelMatrices;
unsigned int star_push_interval = 250;
int trajectory = 1;
int collisionDetectionFlag = 0;
int helicopterDraw = 1;
int musicFlag = 1;
extern GLuint star_instance_buffer;

int moveFlag = 1;
int normalMapFlag = 1;
int multiTexMapFlag = 1;
int viewpoint = 0;
int FogFlag = 1;
int FogColor = 0;
const float FogDensity = 0.025;
const float FogGradient = 2;

void Mouse_Wheel_Func(int button, int state, int x, int y)
{
	if ((button == 3) || (button == 4))
	{
		if (state == GLUT_UP) return;
		if (button == 3)
		{
			radius -= 1.0f;
			cameraX = radius* cos(glm::radians(initViewHorizontal + viewRotateDegree[1]))*sin(glm::radians(initViewVertical + viewRotateDegree[0]));
			cameraY = radius* cos(glm::radians(initViewVertical + viewRotateDegree[0]));
			cameraZ = radius* sin(glm::radians(initViewHorizontal + viewRotateDegree[1]))*sin(glm::radians(initViewVertical + viewRotateDegree[0]));
		}
		else
		{
			radius += 1.0f;
			cameraX = radius* cos(glm::radians(initViewHorizontal + viewRotateDegree[1]))*sin(glm::radians(initViewVertical + viewRotateDegree[0]));
			cameraY = radius* cos(glm::radians(initViewVertical + viewRotateDegree[0]));
			cameraZ = radius* sin(glm::radians(initViewHorizontal + viewRotateDegree[1]))*sin(glm::radians(initViewVertical + viewRotateDegree[0]));
		}
	}
}

void LoadAllTextures()
{
	TextureEarth[0] = loadBMP2Texture("texture/earth.bmp");
	TextureEarth[1] = loadBMP2Texture("normal_map/earth_normal.bmp");
	TextureApple = loadBMP2Texture("texture/apple.bmp");
	TextureGlass[0] = loadBMP2Texture("texture/glass_a.bmp");
	TextureGlass[1] = loadBMP2Texture("texture/grass.bmp");
	TextureHelicopter = loadBMP2Texture("texture/helicopter.bmp");
	std::vector<const GLchar*> skybox_faces;
	skybox_faces.push_back("texture/sea_skybox/right.bmp");
	skybox_faces.push_back("texture/sea_skybox/left.bmp");
	skybox_faces.push_back("texture/sea_skybox/bottom.bmp");
	skybox_faces.push_back("texture/sea_skybox/top.bmp");
	skybox_faces.push_back("texture/sea_skybox/back.bmp");
	skybox_faces.push_back("texture/sea_skybox/front.bmp");
	TextureSkybox = loadCubemap(skybox_faces);
	TextureRock = loadBMP2Texture("texture/brickWall.bmp");
	TextureStar = loadBMP2Texture("texture/starfy.bmp");
}

void sendDataToOpenGL()
{
	//Load objects and bind to VAO & VBO
	bindEarth("model_obj/planet.obj");
	bindHelicopter("model_obj/helicopter2.obj");
	bindSkybox();
	bindRock("model_obj/rock.obj");
	bindStar("model_obj/starfy.obj");
	// load all textures
	LoadAllTextures();
}

void set_lighting(GLint program_ID)
{
	glUseProgram(program_ID);

	// ambient
	GLint ambientLightUniformLocation = glGetUniformLocation(program_ID, "ambientLight");
	glm::vec3 ambientLight(a_brightness, a_brightness, a_brightness);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	// diffusion
	GLint kd = glGetUniformLocation(program_ID, "coefficient_d");
	glm::vec3 vec_kd(d_brightness, d_brightness, d_brightness);
	glUniform3fv(kd, 1, &vec_kd[0]);
	// specular
	GLint ks = glGetUniformLocation(program_ID, "coefficient_s");
	glm::vec3 vec_ks(s_brightness, s_brightness, s_brightness);
	glUniform3fv(ks, 1, &vec_ks[0]);
	// eye position
	GLint eyePositionUniformLocation = glGetUniformLocation(program_ID, "eyePositionWorld");
	vec3 eyePosition(cameraTransX, cameraTransY, cameraTransZ);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);
	// lightbox 1 position
	GLint lightPositionUniformLocation_1 = glGetUniformLocation(program_ID, "lightPositionWorld_1");
	glUniform3fv(lightPositionUniformLocation_1, 1, &lightbox1_pos[0]);
	// lightbox 1 color
	GLint lightColorUniformLocation_1 = glGetUniformLocation(program_ID, "lightColor_1");
	glUniform3fv(lightColorUniformLocation_1, 1, &lightbox1_color[0]);
	// lightbox 2 position
	GLint lightPositionUniformLocation_2 = glGetUniformLocation(program_ID, "lightPositionWorld_2");
	glUniform3fv(lightPositionUniformLocation_2, 1, &lightbox2_pos[0]);
	// lightbox 2 color
	GLint lightColorUniformLocation_2 = glGetUniformLocation(program_ID, "lightColor_2");
	glUniform3fv(lightColorUniformLocation_2, 1, &lightbox2_color[0]);
}

void drawSkybox(void)
{
	// draw skybox
	// Remember to turn depth writing off
	glDepthMask(GL_FALSE);
	glUseProgram(Skybox_programID);

	GLuint Skb_ModelUniformLocation = glGetUniformLocation(Skybox_programID, "M");
	glm::mat4 Skb_ModelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(Skb_ModelUniformLocation, 1, GL_FALSE, &Skb_ModelMatrix[0][0]);
	// Remove any translation component of the view matrix
	glm::mat4 view = glm::mat4(glm::mat3(common_viewM));
	GLint V_ID = glGetUniformLocation(Skybox_programID, "view");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &view[0][0]);
	GLint P_ID = glGetUniformLocation(Skybox_programID, "projection");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	GLint VM_ID = glGetUniformLocation(Skybox_programID, "VM");
	glUniformMatrix4fv(VM_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint FogFlag_ID = glGetUniformLocation(Skybox_programID, "fog_flag");
	GLint FogDensity_ID = glGetUniformLocation(Skybox_programID, "FogDensity");
	GLint FogGradient_ID = glGetUniformLocation(Skybox_programID, "FogGradient");
	GLint FogColor_ID = glGetUniformLocation(Skybox_programID, "FogColor");
	glUniform1i(FogFlag_ID, FogFlag);
	glUniform1f(FogDensity_ID, FogDensity);
	glUniform1f(FogGradient_ID, FogGradient);
	glUniform1i(FogColor_ID, FogColor);

	// skybox cube
	glBindVertexArray(skyboxVao);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(Skybox_programID, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureSkybox);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}

void drawEarth(void)
{
	//earth
	GLfloat scale_fact = 2.5f;
	glUseProgram(programID);

	glBindVertexArray(earthVao);
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(earth_innRot_Degree), glm::vec3(0, 1, 0));
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 3.0f, 0.0f));
	glm::mat4 Model = trans_M * rot_M * scale_M;

	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	glm::mat3 ModelView3x3Matrix = glm::mat3(common_viewM * Model);
	GLuint ModelView3x3MatrixID = glGetUniformLocation(programID, "MV3x3");
	glUniformMatrix3fv(ModelView3x3MatrixID, 1, GL_FALSE, &ModelView3x3Matrix[0][0]);
	
	GLint FogFlag_ID = glGetUniformLocation(programID, "fog_flag");
	GLint FogDensity_ID = glGetUniformLocation(programID, "FogDensity");
	GLint FogGradient_ID = glGetUniformLocation(programID, "FogGradient");
	GLint FogColor_ID = glGetUniformLocation(programID, "FogColor");
	glUniform1i(FogFlag_ID, FogFlag);
	glUniform1f(FogDensity_ID, FogDensity);
	glUniform1f(FogGradient_ID, FogGradient);
	glUniform1i(FogColor_ID, FogColor);

	// texture
	GLuint TextureID_0 = glGetUniformLocation(programID, "myTextureSampler_1");
	GLuint TextureID_1 = glGetUniformLocation(programID, "myTextureSampler_2");
	GLuint normalMap = glGetUniformLocation(programID, "normalMapping_flag");
	GLuint multiTexMap = glGetUniformLocation(programID, "multiTexMapping_flag");
	glUniform1i(normalMap, normalMapFlag);
	glUniform1i(multiTexMap, 0);
	// Bind texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureEarth[0]);
	glUniform1i(TextureID_0, 0);
	// Bind texture in Texture Unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureEarth[1]);
	glUniform1i(TextureID_1, 1);
	// Draw
	glDrawElements(GL_TRIANGLES, drawEarthSize, GL_UNSIGNED_SHORT, 0);
}

void drawApple(void)
{
	//apple
	GLfloat scale_fact = 0.3f;

	glUseProgram(programID);

	glBindVertexArray(earthVao);
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	glm::mat4 inn_rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(apple_innRot_Degree), glm::vec3(0, 1, 0));
	glm::mat4 inn_trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 0.0f, 0.0f));
	glm::mat4 ext_rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(apple_extRot_Degree), glm::vec3(0, 1, 0));
	glm::mat4 ext_trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 3.0f, 0.0f));
	glm::mat4 Model = ext_trans_M * ext_rot_M * inn_trans_M * inn_rot_M * scale_M;

	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	GLint FogFlag_ID = glGetUniformLocation(programID, "fog_flag");
	GLint FogDensity_ID = glGetUniformLocation(programID, "FogDensity");
	GLint FogGradient_ID = glGetUniformLocation(programID, "FogGradient");
	GLint FogColor_ID = glGetUniformLocation(programID, "FogColor");
	glUniform1i(FogFlag_ID, FogFlag);
	glUniform1f(FogDensity_ID, FogDensity);
	glUniform1f(FogGradient_ID, FogGradient);
	glUniform1i(FogColor_ID, FogColor);

	// texture
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler_1");
	GLuint normalMap = glGetUniformLocation(programID, "normalMapping_flag");
	GLuint multiTexMap = glGetUniformLocation(programID, "multiTexMapping_flag");
	glUniform1i(normalMap, 0);
	glUniform1i(multiTexMap, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureApple);
	glUniform1i(TextureID, 0);

	glDrawElements(GL_TRIANGLES, drawEarthSize, GL_UNSIGNED_SHORT, 0);
}

void drawGlass(void)
{
	//glass
	GLfloat scale_fact = 1.0f;

	glUseProgram(programID);

	glBindVertexArray(earthVao);
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(glass_innRot_Degree), glm::vec3(0, 1, 0));
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, -8.0f, 0.0f));
	glassModel = trans_M * rot_M * scale_M;

	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &glassModel[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	GLint FogFlag_ID = glGetUniformLocation(programID, "fog_flag");
	GLint FogDensity_ID = glGetUniformLocation(programID, "FogDensity");
	GLint FogGradient_ID = glGetUniformLocation(programID, "FogGradient");
	GLint FogColor_ID = glGetUniformLocation(programID, "FogColor");
	glUniform1i(FogFlag_ID, FogFlag);
	glUniform1f(FogDensity_ID, FogDensity);
	glUniform1f(FogGradient_ID, FogGradient);
	glUniform1i(FogColor_ID, FogColor);

	// texture
	GLuint TextureID_0 = glGetUniformLocation(programID, "myTextureSampler_1");
	GLuint TextureID_1 = glGetUniformLocation(programID, "myTextureSampler_2");
	GLuint normalMap = glGetUniformLocation(programID, "normalMapping_flag");
	GLuint multiTexMap = glGetUniformLocation(programID, "multiTexMapping_flag");
	glUniform1i(normalMap, 0);
	glUniform1i(multiTexMap, multiTexMapFlag);
	// Bind texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureGlass[0]);
	glUniform1i(TextureID_0, 0);
	// Bind texture in Texture Unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureGlass[1]);
	glUniform1i(TextureID_1, 1);
	// Draw
	glDrawElements(GL_TRIANGLES, drawEarthSize, GL_UNSIGNED_SHORT, 0);
}

void drawLightbox(void)
{
	//lightbox
	GLfloat scale_fact = 0.3f;

	glUseProgram(Lightbox_programID);

	glBindVertexArray(skyboxVao);
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -10.0f, 10.0f));
	glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(lightbox_extRot_Degree), glm::vec3(1, 0, 0));
	glm::mat4 Model =  rot_M * trans_M * scale_M;
	lightbox1_pos = glm::vec3(Model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	GLint M_ID = glGetUniformLocation(Lightbox_programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(Lightbox_programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(Lightbox_programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);
	GLint lightColor_ID = glGetUniformLocation(Lightbox_programID, "lightColor");
	glUniform3fv(lightColor_ID, 1, &lightbox1_color[0]);

	GLint FogFlag_ID = glGetUniformLocation(Lightbox_programID, "fog_flag");
	GLint FogDensity_ID = glGetUniformLocation(Lightbox_programID, "FogDensity");
	GLint FogGradient_ID = glGetUniformLocation(Lightbox_programID, "FogGradient");
	GLint FogColor_ID = glGetUniformLocation(Lightbox_programID, "FogColor");
	glUniform1i(FogFlag_ID, FogFlag);
	glUniform1f(FogDensity_ID, FogDensity);
	glUniform1f(FogGradient_ID, FogGradient);
	glUniform1i(FogColor_ID, FogColor);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -5.0f, 5.0f));
	Model = trans_M * scale_M;
	lightbox2_pos = glm::vec3(Model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	glUniform3fv(lightColor_ID, 1, &lightbox2_color[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawHelicopter(void)
{
	//glass
	GLfloat scale_fact = 0.15f;

	glUseProgram(programID);

	glBindVertexArray(helicopterVao);
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	glm::mat4 y_rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0));
	glm::mat4 inn_trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(helicopter_radius, 0.0f, 0.0f));
	glm::mat4 ext_rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(helicopter_extRot_Degree), glm::vec3(0, 0, 1));
	glm::mat4 ext_trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 6.0f, 0.0f));
	helicopterModel = ext_trans_M * ext_rot_M * inn_trans_M * y_rot_M * scale_M;

	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &helicopterModel[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	GLint FogFlag_ID = glGetUniformLocation(programID, "fog_flag");
	GLint FogDensity_ID = glGetUniformLocation(programID, "FogDensity");
	GLint FogGradient_ID = glGetUniformLocation(programID, "FogGradient");
	GLint FogColor_ID = glGetUniformLocation(programID, "FogColor");
	glUniform1i(FogFlag_ID, FogFlag);
	glUniform1f(FogDensity_ID, FogDensity);
	glUniform1f(FogGradient_ID, FogGradient);
	glUniform1i(FogColor_ID, FogColor);

	// texture
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler_1");
	GLuint normalMap = glGetUniformLocation(programID, "normalMapping_flag");
	GLuint multiTexMap = glGetUniformLocation(programID, "multiTexMapping_flag");
	glUniform1i(normalMap, 0);
	glUniform1i(multiTexMap, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureHelicopter);
	glUniform1i(TextureID, 0);

	if (viewpoint <= 2 && helicopterDraw)
		glDrawArrays(GL_TRIANGLES, 0, drawHelicopterSize);
}

void drawRock(void)
{
	//rock
	
	glUseProgram(Rock_programID);

	glBindVertexArray(rockVao);
	glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(glass_innRot_Degree), glm::vec3(0, 1, 0));
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, -8.0f, 0.0f));
	glm::mat4 Model = trans_M * rot_M;

	GLint M_ID = glGetUniformLocation(Rock_programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(Rock_programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(Rock_programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	GLint FogFlag_ID = glGetUniformLocation(programID, "fog_flag");
	GLint FogDensity_ID = glGetUniformLocation(programID, "FogDensity");
	GLint FogGradient_ID = glGetUniformLocation(programID, "FogGradient");
	GLint FogColor_ID = glGetUniformLocation(programID, "FogColor");
	glUniform1i(FogFlag_ID, FogFlag);
	glUniform1f(FogDensity_ID, FogDensity);
	glUniform1f(FogGradient_ID, FogGradient);
	glUniform1i(FogColor_ID, FogColor);

	// texture
	GLuint TextureID = glGetUniformLocation(Rock_programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureRock);
	glUniform1i(TextureID, 0);

	glDrawArraysInstanced(GL_TRIANGLES, 0, drawRockSize, rockAmount);
}

void drawStar(void)
{
	//star
	glUseProgram(Rock_programID);

	glBindVertexArray(starVao);
	glm::mat4 Model = glm::mat4(1.0f);

	GLint M_ID = glGetUniformLocation(Rock_programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(Rock_programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(Rock_programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	GLint FogFlag_ID = glGetUniformLocation(programID, "fog_flag");
	GLint FogDensity_ID = glGetUniformLocation(programID, "FogDensity");
	GLint FogGradient_ID = glGetUniformLocation(programID, "FogGradient");
	GLint FogColor_ID = glGetUniformLocation(programID, "FogColor");
	glUniform1i(FogFlag_ID, FogFlag);
	glUniform1f(FogDensity_ID, FogDensity);
	glUniform1f(FogGradient_ID, FogGradient);
	glUniform1i(FogColor_ID, FogColor);

	if (starAmount > 0 && trajectory)
	{
		glBindBuffer(GL_ARRAY_BUFFER, star_instance_buffer);
		glBufferData(GL_ARRAY_BUFFER, maxStarAmount * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, starAmount * sizeof(glm::mat4), &starModelMatrices[0]);

		// texture
		GLuint TextureID = glGetUniformLocation(Rock_programID, "myTextureSampler");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureStar);
		glUniform1i(TextureID, 0);

		glDrawArraysInstanced(GL_TRIANGLES, 0, drawStarSize, starAmount);
	}
	
}

void collisionDetection()
{
	glm::vec4 *glassTransVertices, *helicopterTransVertices;
	glassTransVertices = new glm::vec4[earthVertices.size()];
	helicopterTransVertices = new glm::vec4[helicopterVertices.size()];
	for (unsigned int i = 0; i < earthVertices.size(); i++)
	{
		glassTransVertices[i] = glassModel * glm::vec4(earthVertices[i], 1);
	}
	for (unsigned int i = 0; i < helicopterVertices.size(); i++)
	{
		helicopterTransVertices[i] = helicopterModel * glm::vec4(helicopterVertices[i], 1);
	}

	float glassMinX, glassMaxX, glassMinY, glassMaxY, glassMinZ, glassMaxZ;
	float helicopterMinX, helicopterMaxX, helicopterMinY, helicopterMaxY, helicopterMinZ, helicopterMaxZ;
	glassMinX = glassMaxX = glassTransVertices[0].x;
	glassMinY = glassMaxY = glassTransVertices[0].y;
	glassMinZ = glassMaxZ = glassTransVertices[0].z;
	helicopterMinX = helicopterMaxX = helicopterTransVertices[0].x;
	helicopterMinY = helicopterMaxY = helicopterTransVertices[0].y;
	helicopterMinZ = helicopterMaxZ = helicopterTransVertices[0].z;
	for (unsigned int i = 1; i < earthVertices.size(); i++)
	{
		if (glassTransVertices[i].x < glassMinX)
			glassMinX = glassTransVertices[i].x;
		if (glassTransVertices[i].x > glassMaxX)
			glassMaxX = glassTransVertices[i].x;
		if (glassTransVertices[i].y < glassMinY)
			glassMinY = glassTransVertices[i].y;
		if (glassTransVertices[i].y > glassMaxY)
			glassMaxY = glassTransVertices[i].y;
		if (glassTransVertices[i].z < glassMinZ)
			glassMinZ = glassTransVertices[i].z;
		if (glassTransVertices[i].z > glassMaxZ)
			glassMaxZ = glassTransVertices[i].z;
	}
	for (unsigned int i = 1; i < helicopterVertices.size(); i++)
	{
		if (helicopterTransVertices[i].x < helicopterMinX)
			helicopterMinX = helicopterTransVertices[i].x;
		if (helicopterTransVertices[i].x > helicopterMaxX)
			helicopterMaxX = helicopterTransVertices[i].x;
		if (helicopterTransVertices[i].y < helicopterMinY)
			helicopterMinY = helicopterTransVertices[i].y;
		if (helicopterTransVertices[i].y > helicopterMaxY)
			helicopterMaxY = helicopterTransVertices[i].y;
		if (helicopterTransVertices[i].z < helicopterMinZ)
			helicopterMinZ = helicopterTransVertices[i].z;
		if (helicopterTransVertices[i].z > helicopterMaxZ)
			helicopterMaxZ = helicopterTransVertices[i].z;
	}

	if ((glassMinX <= helicopterMaxX && glassMaxX >= helicopterMinX) &&
		(glassMinY <= helicopterMaxY && glassMaxY >= helicopterMinY) &&
		(glassMinZ <= helicopterMaxZ && glassMaxZ >= helicopterMinZ))
		helicopterDraw = 0;
}

void musicControl(int i)
{
	se->setAllSoundsPaused(!musicFlag);
}

void paintGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// ================================ //
	// view matrix
	glm::vec3 cameraPos;
	glm::vec3 lookAtPos;
	glm::vec3 upVector;
	switch (viewpoint)
	{
	case 0:
		cameraTransX = cameraX + cameraDeltaX;
		cameraTransY = cameraY + cameraDeltaY;
		cameraTransZ = cameraZ;
		cameraPos = glm::vec3(cameraTransX, cameraTransY, cameraTransZ);
		lookAtPos = glm::vec3(cameraDeltaX, cameraDeltaY, 0);
		upVector = glm::vec3(0, 1, 0);
		break;
	case 1:
		cameraTransX = cameraX + cameraDeltaX;
		cameraTransY = cameraZ;
		cameraTransZ = -(cameraY + cameraDeltaY);
		cameraPos = glm::vec3(cameraTransX, cameraTransY, cameraTransZ);
		lookAtPos = glm::vec3(cameraDeltaX, 0, -cameraDeltaY);
		upVector = glm::vec3(0, 0, -1);
		break;
	case 2:
		cameraTransX = cameraZ;
		cameraTransY = cameraY + cameraDeltaY;
		cameraTransZ = -(cameraX + cameraDeltaX);
		cameraPos = glm::vec3(cameraTransX, cameraTransY, cameraTransZ);
		lookAtPos = glm::vec3(0, cameraDeltaY, -cameraDeltaX);
		upVector = glm::vec3(0, 1, 0);
		break;
	case 3:
		cameraPos = glm::vec3(helicopterModel * glm::vec4(0, 0, 0, 1));
		cameraTransX = cameraPos.x;
		cameraTransY = cameraPos.y;
		cameraTransZ = cameraPos.z;
		lookAtPos = glm::vec3(helicopterModel * glm::vec4(0, 1, 0, 1));
		upVector = glm::vec3(helicopterModel * glm::vec4(0, 0, 1, 0));
		break;
	case 4:
		cameraPos = glm::vec3(helicopterModel * glm::vec4(0, 0, 0, 1));
		cameraTransX = cameraPos.x;
		cameraTransY = cameraPos.y;
		cameraTransZ = cameraPos.z;
		lookAtPos = glm::vec3(helicopterModel * glm::vec4(0, -1, 0, 1));
		upVector = glm::vec3(helicopterModel * glm::vec4(0, 0, 1, 0));
		break;
	case 5:
		cameraPos = glm::vec3(helicopterModel * glm::vec4(0, 0, 0, 1));
		cameraTransX = cameraPos.x;
		cameraTransY = cameraPos.y;
		cameraTransZ = cameraPos.z;
		lookAtPos = glm::vec3(helicopterModel * glm::vec4(0, 0, -1, 1));
		upVector = glm::vec3(helicopterModel * glm::vec4(0, 1, 0, 0));
		break;
	}
	common_viewM = glm::lookAt(cameraPos, lookAtPos, upVector);
	// projection matrix
	common_projection = glm::perspective(camera_fov, (float)Win_w/(float)Win_h, 0.1f, 200.0f);
	
	drawSkybox();

	//=== draw ===//
	drawLightbox();
	// set lighting parameters
	set_lighting(programID);
	// draw earth
	drawEarth();
	drawApple();
	drawGlass();
	drawHelicopter();

	set_lighting(Rock_programID);
	drawRock();
	drawStar();

	if (collisionDetectionFlag)
		collisionDetection();
	glui->sync_live();

	glutSwapBuffers();
	glutPostRedisplay();
}

void Shaders_Installer()
{
	programID = installShaders("shader/Common.vs", "shader/Common.frag");
	Lightbox_programID = installShaders("shader/Lightbox.vs", "shader/Lightbox.frag");
	Skybox_programID = installShaders("shader/Skybox.vs", "shader/Skybox.frag");
	Rock_programID = installShaders("shader/Rock.vs", "shader/Rock.frag");

	if (!checkProgramStatus(programID))
		return;
	if (!checkProgramStatus(Lightbox_programID))
		return;
	if (!checkProgramStatus(Skybox_programID))
		return;
	if (!checkProgramStatus(Rock_programID))
		return;
}

void initializedGL(void)
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	// install all shaders
	Shaders_Installer();
	// load required obj and textures
	sendDataToOpenGL();
}

void timerFunction(int id)
{
	if (moveFlag == 1)
	{
		earth_innRot_Degree += 0.3;
		apple_innRot_Degree -= 0.8;
		apple_extRot_Degree -= 0.5;
		glass_innRot_Degree += 0.4;
		lightbox_extRot_Degree -= 0.5;
		if (helicopterDraw)
			helicopter_extRot_Degree += helicopter_speed;

		glutPostRedisplay();
	}
	glutTimerFunc(700.0f / 60.0f, timerFunction, 1);
}

void starModelMatricesTimerFunc(int id)
{
	if (moveFlag && helicopterDraw)
	{
		if (helicopterModel != glm::mat4(0.0f))
		{
			if (starAmount == maxStarAmount)
			{
				if (starLastUsed == maxStarAmount - 1)
				{
					starModelMatrices[0] = helicopterModel;
					starLastUsed = 0;
				}
				else
				{
					starLastUsed += 1;
					starModelMatrices[starLastUsed] = helicopterModel;
				}
			}
			else
			{
				starLastUsed += 1;
				starAmount += 1;
				starModelMatrices[starLastUsed] = helicopterModel;
			}
		}
	}
	glutTimerFunc(star_push_interval, starModelMatricesTimerFunc, 1);
}

void myGlutReshape(int w, int h)
{
	int tx, ty, tw, th;
	GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
	glViewport(tx, ty, tw, th);
	Win_w = tw;
	Win_h = th;
}

void resetCamera(int i)
{
	radius = initRadius;
	cameraX = 0;
	cameraY = 0;
	cameraZ = radius;
	cameraDeltaX = 0;
	cameraDeltaY = 0;
}

void resetHelicopter(int i)
{
	helicopter_radius = helicopter_init_radius;
	helicopter_speed = helicopter_init_speed;
	starAmount = 0;
	starLastUsed = -1;
	helicopterDraw = 1;
}

void Keyboard_func(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		viewpoint = 0;
		resetCamera(0);
		break;
	case 's':
		viewpoint = 1;
		resetCamera(0);
		break;
	case 'd':
		viewpoint = 2;
		resetCamera(0);
		break;
	case 32:
		if (mouseControl)
			mouseControl = false;
		else
			mouseControl = true;
		break;
	}
}

void Special_func(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		helicopter_speed += 0.1;
		break;
	case GLUT_KEY_DOWN:
		helicopter_speed -= 0.1;
		break;
	case GLUT_KEY_LEFT:
		helicopter_radius += 0.25f;
		break;
	case GLUT_KEY_RIGHT:
		helicopter_radius -= 0.25f;
		break;
	}
}

void PassiveMouse_func(int x, int y)
{
	if (mouseLastX == -1 || mouseLastY == -1)
	{
		mouseLastX = x;
		mouseLastY = y;
		return;
	}
	if (mouseControl)
	{
		cameraDeltaX += (x - mouseLastX) * mouseSensitivity;
		cameraDeltaY += (mouseLastY - y) * mouseSensitivity;
	}
	mouseLastX = x;
	mouseLastY = y;
}

int main(int argc, char *argv[])
{
	/*Initialization of GLUT library*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	int width = GetSystemMetrics(SM_CXSCREEN)*0.8;
	int height = GetSystemMetrics(SM_CYSCREEN)*0.8;
	glutInitWindowPosition((int) (width * 0.1), (int) (height * 0.1));
	glutInitWindowSize(width, height);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	mainWindowID = glutCreateWindow("i-Navigation");

	starModelMatrices = new glm::mat4[maxStarAmount];

	// initialize openGL
	initializedGL();
	// draw
	glutDisplayFunc(paintGL);

	// GLUI
	GLUI_Master.set_glutKeyboardFunc(Keyboard_func);
	GLUI_Master.set_glutSpecialFunc(Special_func);
	GLUI_Master.set_glutMouseFunc(Mouse_Wheel_Func);
	GLUI_Master.set_glutReshapeFunc(myGlutReshape);
	glui = GLUI_Master.create_glui_subwindow(mainWindowID, GLUI_SUBWINDOW_RIGHT);
	glui->set_main_gfx_window(mainWindowID);
	glui->add_separator();
	GLUI_StaticText *infoText1 = glui->add_statictext("i-Navigation");
	infoText1->set_alignment(GLUI_ALIGN_CENTER);
	glui->add_separator();
	GLUI_StaticText *infoText2 = glui->add_statictext("CSCI3260 Course Project");
	infoText2->set_alignment(GLUI_ALIGN_CENTER);
	GLUI_StaticText *infoText3 = glui->add_statictext("Group 35");
	infoText3->set_alignment(GLUI_ALIGN_CENTER);
	GLUI_StaticText *infoText4 = glui->add_statictext("Kwan Chun Tat");
	infoText4->set_alignment(GLUI_ALIGN_CENTER);
	GLUI_StaticText *infoText5 = glui->add_statictext("1155033423");
	infoText5->set_alignment(GLUI_ALIGN_CENTER);
	glui->add_separator();
	GLUI_Spinner *helicopterSpeedSpinner = glui->add_spinner("HelicopterSpeed:", GLUI_SPINNER_FLOAT, &helicopter_speed);
	helicopterSpeedSpinner->set_speed(0.1);
	glui->add_button("Reset Helicopter", 0, resetHelicopter);
	GLUI_Rollout *renderControlRollout = glui->add_rollout("Render Control");
	GLUI_Panel *moveAndMapPanel = glui->add_panel_to_panel(renderControlRollout, "Move/Map");
	glui->add_checkbox_to_panel(moveAndMapPanel, "Move", &moveFlag);
	glui->add_checkbox_to_panel(moveAndMapPanel, "MouseControl", &mouseControl);
	glui->add_checkbox_to_panel(moveAndMapPanel, "NormalMap", &normalMapFlag);
	glui->add_checkbox_to_panel(moveAndMapPanel, "MultiTexMap", &multiTexMapFlag);
	glui->add_checkbox_to_panel(moveAndMapPanel, "Trajectory", &trajectory);
	glui->add_checkbox_to_panel(moveAndMapPanel, "CollisionDetection", &collisionDetectionFlag);
	GLUI_Panel *viewpointPanel = glui->add_panel_to_panel(renderControlRollout, "Viewpoint");
	GLUI_RadioGroup *viewpointGroup = glui->add_radiogroup_to_panel(viewpointPanel, &viewpoint, 0, resetCamera);
	glui->add_radiobutton_to_group(viewpointGroup, "Main");
	glui->add_radiobutton_to_group(viewpointGroup, "Top");
	glui->add_radiobutton_to_group(viewpointGroup, "Right");
	glui->add_radiobutton_to_group(viewpointGroup, "Helicopter Front");
	glui->add_radiobutton_to_group(viewpointGroup, "Helicopter Tail");
	glui->add_radiobutton_to_group(viewpointGroup, "Helicopter Bottom");
	GLUI_Panel *FogOnOffPanel = glui->add_panel_to_panel(renderControlRollout, "Fog On/Off");
	glui->add_checkbox_to_panel(FogOnOffPanel, "Fog", &FogFlag);
	GLUI_Panel *FogColorPanel = glui->add_panel_to_panel(renderControlRollout, "Fog Color");
	GLUI_RadioGroup *FogColorGroup = glui->add_radiogroup_to_panel(FogColorPanel, &FogColor);
	glui->add_radiobutton_to_group(FogColorGroup, "Grey");
	glui->add_radiobutton_to_group(FogColorGroup, "Orange");
	glui->add_separator();
	GLUI_Rollout *musicControlRollout = glui->add_rollout("Music Control");
	glui->add_checkbox_to_panel(musicControlRollout, "Music", &musicFlag, 0, musicControl);
	glutPassiveMotionFunc(PassiveMouse_func);
	glutTimerFunc(700.0f / 60.0f, timerFunction, 1);
	glutTimerFunc(star_push_interval, starModelMatricesTimerFunc, 1);

	se = createIrrKlangDevice();
	if (!se)
	{
		printf("Error: Could not create Sound Engine\n");
		return 0; // error starting up the engine
	}
	se->play2D("music/getout.ogg", true);

	/*Enter the GLUT event processing loop which never returns.*/
	glutMainLoop();

	return 0;
}