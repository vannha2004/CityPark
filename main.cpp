#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Vector_Matrix.h>
#include<ctime>//Lấy thời gian hiện tại
#include "stb_image.h"

using namespace std;
//Thêm tam giác màu
static const float coloredTriangle[] = {
	//position			//colors
	 1.0, -1.0,  0.0,	1.0, 0.5, 0.0,//bottom right
	-1.0, -1.0,  0.0,	0.0, 1.0, 0.0,//bottom left
	 0.0,  1.0,  0.0,	0.0, 0.3, 1.0//top
};
typedef struct
{
	float xyzw[4];
	float RGBA[4];
	float texture[4];
	float normal[4];
} Vertex;

Vertex Vertices[] =
{
	// v0-v1-v2 (front)
	{ {  0.5f,	 0.5f,	0.5f, 1.0 },{ 0, 0, 1, 0 } },
	{ { -0.5f,	 0.5f,	0.5f, 1.0 },{ 0, 0, 1, 0 } },
	{ { -0.5f,	-0.5f,	0.5f, 1.0 },{ 0, 0, 1, 0 } },
	// v2-v3-v0
	{ { -0.5f,	-0.5f,	0.5f, 1.0 },{ 0, 0, 0.5, 0 } },
	{ {  0.5f,	-0.5f,	0.5f, 1.0 },{ 0, 0, 0.5, 0 } },
	{ {  0.5f,	 0.5f,	0.5f, 1.0 },{ 0, 0, 0.5, 0 } },

	// v0-v3-v4 (right)
	{ { 0.5f,	 0.5f,	 0.5f, 1.0 },{ 1, 0, 0.2, 0 } },
	{ { 0.5f,	-0.5f,   0.5f, 1.0 },{ 1, 0, 0.2, 0 } },
	{ { 0.5f,	-0.5f,  -0.5f, 1.0 },{ 1, 0, 0.2, 0 } },
	// v4-v5-v0
	{ { 0.5f,	-0.5f,	-0.5f, 1.0 },{ 1, 0, 0, 0 } },
	{ { 0.5f,	 0.5f,	-0.5f, 1.0 },{ 1, 0, 0, 0 } },
	{ { 0.5f,	 0.5f,   0.5f, 1.0 },{ 1, 0, 0, 0 } },

	// v0-v5-v6 (top)
	{ {  0.5f,	0.5f,	 0.5f, 1.0 },{ 0, 1, 0, 0 } },
	{ {  0.5f,	0.5f,	-0.5f, 1.0 },{ 0, 1, 0, 0 } },
	{ { -0.5f,	0.5f,	-0.5f, 1.0 },{ 0, 1, 0, 0 } },
	// v6-v1-v0
	{ { -0.5f,	0.5f,	-0.5f, 1.0 },{ 0, 1, 0, 0 } },
	{ { -0.5f,	0.5f,	 0.5f, 1.0 },{ 0, 1, 0, 0 } },
	{ {  0.5f,	0.5f,	 0.5f, 1.0 },{ 0, 1, 0, 0 } },

	// v1-v6-v7 (left)
	{ { -0.5f,	 0.5f,	 0.5f, 1.0 },{ -1, 0, 0.7, 0 } },
	{ { -0.5f,	 0.5f,	-0.5f, 1.0 },{ -1, 0, 0.7, 0 } },
	{ { -0.5f,	-0.5f,	-0.5f, 1.0 },{ -1, 0, 0.7, 0 } },
	// v7-v2-v1
	{ { -0.5f,	-0.5f,	-0.5f, 1.0 },{ -1, 0, 0, 0 } },
	{ { -0.5f,	-0.5f,	 0.5f, 1.0 },{ -1, 0, 0, 0 } },
	{ { -0.5f,	 0.5f,	 0.5f, 1.0 },{ -1, 0, 0, 0 } },

	// v7-v4-v3 (bottom)
	{ { -0.5f,	 -0.5f,	-0.5f, 1.0 },{ 0,-1, 0, 0 } },
	{ {  0.5f,	-0.5f,	-0.5f, 1.0 },{ 0,-1, 0, 0 } },
	{ {  0.5f,	-0.5f,	 0.5f, 1.0 },{ 0,-1, 0, 0 } },
	// v3-v2-v7
	{ {  0.5f,	-0.5f,	 0.5f, 1.0 },{ 0,-1, 0.1, 0 } },
	{ { -0.5f,	-0.5f,	 0.5f, 1.0 },{ 0,-1, 0.1, 0 } },
	{ { -0.5f,	-0.5f,	-0.5f, 1.0 },{ 0,-1, 0.1, 0 } },

	// v4-v7-v6 (back)
	{ {  0.5f,	-0.5f,	-0.5f, 1.0 },{ 0, 0,-1, 0 } },
	{ { -0.5f,	-0.5f,	-0.5f, 1.0 },{ 0, 0,-1, 0 } },
	{ { -0.5f,	 0.5f,	-0.5f, 1.0 },{ 0, 0,-1, 0 } },
	// v6-v5-v4
	{ { -0.5f,	 0.5f,	-0.5f, 1.0 },{ 0, 0,-1, 0 } },
	{ {  0.5f,	 0.5f,	-0.5f, 1.0 },{ 0, 0,-1, 0 } },
	{ {  0.5f,	-0.5f,	-0.5f, 1.0 },{ 0, 0,-1, 0 } }
};

const size_t BufferSize = sizeof(Vertices);
const size_t VertexSize = sizeof(Vertices[0]);
const size_t NormalOffset = sizeof(Vertices[0].xyzw);
const size_t RgbOffset = sizeof(Vertices[0].xyzw); //Thêm
const size_t TextureOffset = sizeof(Vertices[0].xyzw) + sizeof(Vertices[1].RGBA) + sizeof(Vertices[1].normal); //Thêm

// Định nghĩa stack cho ma trận
class MatrixStack {
	int    index;
	int    size;
	mat4* matrices;

public:
	MatrixStack(int numMatrices = 32) :index(0), size(numMatrices)
	{
		matrices = new mat4[numMatrices];
	}

	~MatrixStack()
	{
		delete[]matrices;
	}

	mat4& push(const mat4& m) {
		assert(index + 1 < size);
		matrices[index++] = m;
		return matrices[index];
	}

	mat4& pop(void) {
		assert(index - 1 >= 0);
		index--;
		return matrices[index];
	}
};

MatrixStack  mvstack;

int CurrentWidth = 700,
CurrentHeight = 700;

GLuint
VaoId[2],
VboId[2],
VertexShaderId,
FragmentShaderId,
ProgramId;

GLfloat yf = 0, xf = 0;

mat4
model_mat_cpp,
view_mat_cpp,
projection_mat_cpp,
temp;

int
model_mat_location,
view_mat_location,
projection_mat_location;

// Ma trận Xoay, Tỉ lệ, Tịnh tiến
float
R[] = { 0.0f, 0.0f, 0.0f },
S[] = { 1.0f, 1.0f, 1.0f },
T[] = { 0.0f, 0.0f, 0.0f };
//=============Sử dụng cho Camera============
int midWindowX;
int midWindowY;


bool keys[256]; // 256 kí tự trong bảng chữ cái
bool mouseLocked = true;
float beta = 0.0f;
float pitch = 0.0f;
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 5.0f;
float moveSpeed = 0.5f;
float jumpSpeed = 0.4f;
float gravity = 0.010f;
bool isJumping = false;
bool isGrounded = true;

//--------------------------------------------------Dùng kiểm tra tịnh tiến, quay, co giãn--------------------------------------------------
bool is_translated = false,
is_rotated = false,
is_scaled = false;

float Alpha = 0.0f;
float t = 0;
float c = 0;
float z = 0;

bool LoadTexture(const char* file_name, GLuint* texture) {
	int x, y, n;
	int force_channels = 4;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(stderr, "WARNING: texture %s is not power-of-2 dimensions\n",
			file_name);
	}
	glGenTextures(1, texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
	return true;
}

string ReadShaderSourceFile(string fileName) {
	fstream reader(fileName.c_str());
	string line;
	string code = "";
	while (getline(reader, line)) {
		code += line + "\n";
	}
	reader.close();
	return code;
}
// ------------------------------------------
void CreatVaoVbo()
{
	glGenVertexArrays(2, &VaoId[0]);
	glGenBuffers(2, &VboId[0]);
	glBindVertexArray(VaoId[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coloredTriangle), coloredTriangle, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0); // Bật vertex_position
	glEnableVertexAttribArray(1); // Bật vertex_color
	glBindVertexArray(VaoId[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glBufferData(GL_ARRAY_BUFFER, BufferSize, Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, (void*)0);
	// Layout location 2 cho vertex_normal
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)NormalOffset);

	// Bật thuộc tính đỉnh
	glEnableVertexAttribArray(0); // Bật vertex_position
	glEnableVertexAttribArray(2); // Bật vertex_normal

	// Bật kiểm tra độ sâu và ẩn con trỏ chuột
	glEnable(GL_DEPTH_TEST);
	glutSetCursor(GLUT_CURSOR_NONE);
}
// ------------------------------------------
void CreatShaders()
{
	string vertexSrc = ReadShaderSourceFile("./vs.shader");
	string fragmentSrc = ReadShaderSourceFile("./fs.shader");

	const GLchar* VertexShader = vertexSrc.c_str();
	const GLchar* FragmentShader = fragmentSrc.c_str();

	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);
	glLinkProgram(ProgramId);
	glUseProgram(ProgramId);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0); // position
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset); // color
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)NormalOffset); // normal
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)TextureOffset); // texture
}
// ------------------------------------------
void CloseFunc()
{
	glUseProgram(0);

	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);

	glDeleteProgram(ProgramId);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboId[0]);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId[0]);
}
//--------------------------------------------------Ánh sáng--------------------------------------------------
#pragma region light
bool light1 = true, light2 = true;

void updateLightUniforms() {
	//light1 = 1; light2 = 1;
	glUniform1i(glGetUniformLocation(ProgramId, "enable_light_1"), light1 ? 0 : 1);
	glUniform1i(glGetUniformLocation(ProgramId, "enable_light_2"), light2 ? 1 : 0);
}

void onl1() {
	light1 = true;
	updateLightUniforms();
}

void offl1() {
	light1 = false;
	updateLightUniforms();
}


// ************************************Gọi màu trong vs.shader************************************
void setInt(const string& name, int value) {
	glUniform1i(glGetUniformLocation(ProgramId, name.c_str()), value);
}
//-----------Tạo khối hộp cube đơn vị--------
void cube()
{
	mvstack.push(model_mat_cpp);
	mat4 instance = identity_mat4();
	mat4 model_cube = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_cube.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model_mat_cpp = mvstack.pop();
}
//-------------------------------

void ground() {
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(0.0, -1.05, -3.5)) * scale(vec3(60.0, 0.01, 40.0));
	setInt("color", 5);
	cube();
	model_mat_cpp = mvstack.pop();
}

void sky()
{
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(0.0, 50.0, -25.0)) * scale(vec3(150.0, 0.01, 200.0));
	setInt("color", 16);
	cube();
	model_mat_cpp = mvstack.pop();
}

//Các thông số để mở cửa nhà VS
float door_angle = 0.0f, toilet_angle = -90.0f, water_angle = 0.0f;
bool is_rotating = true;
namespace toilet {
	void tuong(float x, float y, float z, float width, float height, float depth) {
		mvstack.push(model_mat_cpp);
		setInt("color", 01);

		mat4 instance = identity_mat4();
		instance = translate(vec3(x, y, z)) * scale(vec3(width, height, depth));

		mat4 model_column = model_mat_cpp * instance;
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_column.m);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		model_mat_cpp = mvstack.pop();
	}
	void canhcua() {
		mvstack.push(model_mat_cpp);
		setInt("color", 14);
		model_mat_cpp = model_mat_cpp
			* translate(vec3(4, -0.2, 4.4))
			* rotate_y(door_angle)
			* translate(vec3(-0.5, 0, 0)) // Đưa cửa về vị trí ban đầu
			* scale(vec3(1, 2, 0.05));        // Thay đổi kích thước cửa

		cube();  // Vẽ cánh cửa
		model_mat_cpp = mvstack.pop();
	}
	void vetuong() {
		tuong(5, -0.2, 5, 0.05, 2, 1.2); //Trai
		tuong(4, -0.2, 5, 0.05, 2, 1.2); //Phai
		tuong(4 + 0.5, -0.2, 5 + 0.6, 1, 2, 0.05); //Sau
		tuong(4 + 0.5, 0.8, 5, 1, 0.05, 1.2); //Tren
		canhcua();

	}
	void toilet(float x, float y, float z, float width, float height, float depth, int id_color) {
		mvstack.push(model_mat_cpp);
		setInt("color", id_color);

		mat4 instance = identity_mat4();
		instance = translate(vec3(x, y, z)) * scale(vec3(width, height, depth));

		mat4 model_column = model_mat_cpp * instance;
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_column.m);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		model_mat_cpp = mvstack.pop();
	}
	void naptoilet() {
		mvstack.push(model_mat_cpp);
		setInt("color", 10);
		model_mat_cpp = model_mat_cpp
			* translate(vec3(4 + 0.5, -0.8, 4.9))
			* rotate_x(toilet_angle)
			* translate(vec3(0, 0, 0.2)) 
			* scale(vec3(0.5, 0.05, 0.25));       

		cube();
		model_mat_cpp = mvstack.pop();
	}
	void xanuoc() {
		mvstack.push(model_mat_cpp);
		setInt("color", 16);
		model_mat_cpp = model_mat_cpp
			* translate(vec3(4.5, -0.9, 4.9))        
			* rotate_y(water_angle)                     
			* translate(vec3(0, 0, 0))                 
			* scale(vec3(0.1, 0.03, 0.1));          
		cube();
		model_mat_cpp = mvstack.pop();
	}

	//Hàm điểu chỉnh tốc độ quay của nước
	void updateWaterAngle(float deltaTime) {
		if (is_rotating) {
			water_angle += 360.0f * deltaTime; // Xoay nước, tốc độ xoay là 360 độ mỗi giây
			if (water_angle >= 360.0f) {
				water_angle -= 360.0f; // Giữ góc quay trong khoảng [0, 360]
			}
		}
	}
	void vetoilet() {
		toilet(4 + 0.5, -0.75, 5, 0.5, 0.5, 0.1, 98); //Cái sau
		toilet(4.76, -1, 4.95, 0.03, 0.3, 0.25, 04); //Cái dưới phải
		toilet(4.24, -1, 4.95, 0.03, 0.3, 0.25, 04); //Cái dưới Trái
		toilet(4.5, -1, 4.84, 0.5, 0.3, 0.03, 04); //Thanh chắn trước
		naptoilet();
		xanuoc();
	}

}

//Quầy kem
float alpha3 = 180.0f;
namespace quaykem {
	void wall_left_right() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.1, 2.5, 4.0));
		setInt("color", 1);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void wall_back() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(3.05, 2.5, 0.1));
		setInt("color", 1);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void wall_top() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(3.05, 0.1, 4));
		setInt("color", 1);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void tam_chan() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(1.5, 0.9, 0.02));
		setInt("color", 14);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void bien_hieu() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(3.05, 0.6, 0.02));
		setInt("color", 4);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void canh_cua() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(1.5, 2.1, 0.02));
		setInt("color", 7);
		cube();
		model_mat_cpp = mvstack.pop();
	}


	void thanh_ngan_cach() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.05, 2, 0.02));
		setInt("color", 03);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	// Hàm LED ánh sáng với chuyển động lên xuống
	void led_light(float time) {
		mvstack.push(model_mat_cpp);

		// Tạo hiệu ứng chuyển động LED lên xuống
		float offset = sin(time * 10.0f) * 0.15f; // Chuyển động theo trục Y (biên độ 0.15)
		model_mat_cpp = model_mat_cpp * translate(vec3(0.0f, offset, 0.0f)) * scale(vec3(0.05, 0.05, 0.05));
		setInt("color", 16); // Màu đèn LED
		cube();

		mvstack.pop();
	}

	// Hàm chuỗi đèn LED quanh biển hiệu
	void led_chain(float time) {
		for (float y = -1.75; y <= 0.2; y += 0.3) {
			mvstack.push(model_mat_cpp);
			model_mat_cpp = model_mat_cpp * translate(vec3(-1.5, y, -0.03)); // Đặt LED ngay trước biển hiệu
			led_light(time); // Hiển thị đèn LED với hiệu ứng
			model_mat_cpp = mvstack.pop();
		}
	}

	void quay_kem(float time) {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(1.5, 0, 0));
		wall_left_right(); //Tuong trai
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-1.5, 0, 0));
		wall_left_right(); //Tuong phai
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(0, 0, 2.0));
		wall_back(); //Tuong sau
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(0, 2.5 / 2, 0));
		wall_top(); //Tuong tren
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(0, 0.95, -2));
		bien_hieu(); //Bien hieu
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(0.75, -0.6, -2));
		tam_chan(); //Tam chan
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-1.52, 0, -2)) * rotate_y(alpha3) * translate(vec3(-1.5 / 2, 0, 0));
		canh_cua(); //Canh cua
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(0, 0, -2));
		thanh_ngan_cach(); //Thanh ngăn cách giữa cửa và chỗ bán
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp); 
		model_mat_cpp = model_mat_cpp * translate(vec3(0, 0.95, -2));
		led_chain(time); //LED phải
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(3.0, 0.95, -2)); 
		led_chain(time); //LED trái
		model_mat_cpp = mvstack.pop();
	}
}
//Đồng hồ BigBag
namespace dongho {
	void thanhdoc() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.08, 20.0, 0.08)); // Phóng to thêm
		setInt("color", 16);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void thanhdoctrencung() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.08, 1.16, 0.08)); // Phóng to thêm
		setInt("color", 16);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void thancot() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(6.0, 20.0, 4.0)); // Phóng to thêm
		setInt("color", 41);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void matdongho() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(6.4, 6.0, 0.2)); 
		setInt("color", 18);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void vongtron(float radius) {
		int numSegments = 12;
		float angleStep = 2.0f * M_PI / numSegments;

		mvstack.push(model_mat_cpp);
		for (int i = 0; i < numSegments; i++) {
			float angle = i * angleStep;
			float x = radius * cos(angle);
			float y = radius * sin(angle);

			mvstack.push(model_mat_cpp);
			model_mat_cpp = model_mat_cpp * translate(vec3(x, y, 0.0f)) * scale(vec3(0.08, 0.08, 0.08));
			setInt("color", 15);
			cube();
			model_mat_cpp = mvstack.pop();
		}
		model_mat_cpp = mvstack.pop();
	}

	void kimdongho() {
		time_t now = time(0);
		struct tm localTime;

		localtime_s(&localTime, &now);

		int gio = localTime.tm_hour % 12;
		int phut = localTime.tm_min;
		int giay = localTime.tm_sec;

		float goc_phut = -6.0f * phut - 0.1f * giay;
		float goc_gio = -30.0f * gio - 0.5f * phut - 0.00833f * giay;


		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(0.0, 0.0, 0.0)) * rotate_z(goc_phut) * translate(vec3(0.0, 0.6, 0.0)); // Di chuyển gốc xoay về đầu kim
		setInt("color", 33);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.04, 1.0, 0.02));
		cube();
		model_mat_cpp = mvstack.pop();

		// Vẽ kim giờ
		mvstack.push(model_mat_cpp);

		model_mat_cpp = model_mat_cpp * translate(vec3(0.0, 0.0, 0.0)) * rotate_z(goc_gio) * translate(vec3(0.0, 0.5, 0.0)); // Di chuyển gốc xoay về đầu kim và kim ngắn hơn
		setInt("color", 34);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.04, 0.9, 0.02));

		cube();
		model_mat_cpp = mvstack.pop();
	}


	void tangtren() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(4, 0.75, 4.5)); // Phóng to thêm

		setInt("color", 10);  // Chọn màu cho tường
		cube();  // Vẽ bức tường
		model_mat_cpp = mvstack.pop();
	}

	void tuongcheo() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.2, 2.0, 5.6)); // Phóng to thêm

		setInt("color", 8);  
		cube();  
		model_mat_cpp = mvstack.pop();
	}

	
	void thanhcheoleft() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * rotate_z(45) * scale(vec3(0.08 * 2, 2.0 * 2, 5.6)); 
		setInt("color", 44);
		cube();
		model_mat_cpp = mvstack.pop();
	}
	void thanhcheoright() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * rotate_z(-45) * scale(vec3(0.08 * 2, 2.0 * 2, 5.6)); 
		setInt("color", 44);  
		cube();  
		model_mat_cpp = mvstack.pop();
	}

	void bigbag() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(10.0, 9, 0));
		thancot();
		model_mat_cpp = mvstack.pop();

		for (float x = 7; x <= 13.25; x += 0.3 * 2) {
			mvstack.push(model_mat_cpp);
			model_mat_cpp = model_mat_cpp * translate(vec3(x, 9, -1 * 2)); 
			thanhdoc();
			model_mat_cpp = mvstack.pop();
		}

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(10.0, 12, -1.04 * 2)); 
		matdongho();
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(10.0, 12, -1.1 * 2)); 
		kimdongho();
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(10.0, 12, -1.1 * 2));
		vongtron(1.8f); // Bán kính vòng tròn
		model_mat_cpp = mvstack.pop();


		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(10, 19.25, 0));
		tangtren();
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(8.6, 20, 0)); 
		thanhcheoright();
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(11.4, 20, 0)); 
		thanhcheoleft();
		model_mat_cpp = mvstack.pop();
	}
}

float alpha_khoanuoc = 0.0f;
bool dropWater = false;    // Trạng thái giọt nước 
float waterPosY = 0.1f;    // Vị trí ban đầu của giọt nước theo trục Y
float waterSpeed = -0.005f; // Tốc độ rơi xuống của giọt nước
namespace voinuoc {
	void thanvoi() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.2, 1.0, 0.5));
		setInt("color", 98);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void khoanuoc() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * rotate_z(alpha_khoanuoc) * scale(vec3(0.05, 0.02, 0.02));
		setInt("color", 3);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void truc1() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.02, 0.2, 0.02));
		setInt("color", 1);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void truc2() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.1, 0.02, 0.02));
		setInt("color", 1);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void truc3() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.02, 0.05, 0.02));
		setInt("color", 1);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void manh_left_right() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.22, 0.2, 0.01));
		setInt("color", 10);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void manh_bot() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.18, 0.01, 0.415));
		setInt("color", 05);
		cube();
		model_mat_cpp = mvstack.pop();
	}

	void manh_front() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.01, 0.18, 0.5));
		setInt("color", 10);
		cube();
		model_mat_cpp = mvstack.pop();
	}


	void water() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * scale(vec3(0.02, 0.06, 0.01));
		setInt("color", 16);
		cube();
		model_mat_cpp = mvstack.pop();
	}
	void updateWater() {
		if (dropWater) {
			waterPosY += waterSpeed; // Cập nhật vị trí Y
			if (waterPosY < -0.5f) { // Giới hạn rơi (dưới một điểm nhất định)
				dropWater = false;   // Dừng giọt nước khi chạm "đất"
			}
		}
	}
	void hoanchinh() {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-3, -0.55, -1.0));
		thanvoi();
		model_mat_cpp = mvstack.pop();

		// Thân vòi 1.1
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-2.95, 0.04, -1.2));
		truc1();
		model_mat_cpp = mvstack.pop();

		// Khóa nước 1
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-2.95, 0.04, -1.21));
		khoanuoc();
		model_mat_cpp = mvstack.pop();

		// Thân vòi 1.2
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-2.895, 0.14, -1.2));
		truc2();
		model_mat_cpp = mvstack.pop();

		// Thân vòi 1.3
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-2.85, 0.12, -1.2));
		truc3();
		model_mat_cpp = mvstack.pop();

		//Giọt nước 1
		if (dropWater) {
			mvstack.push(model_mat_cpp);
			model_mat_cpp = model_mat_cpp * translate(vec3(-2.85, waterPosY, -1.2));
			water();
			model_mat_cpp = mvstack.pop();
		}


		// Thân vòi 2.1
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-2.95, 0.04, -0.81));
		truc1();
		model_mat_cpp = mvstack.pop();

		// Khóa nước 2
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-2.95, 0.04, -0.8));
		khoanuoc();
		model_mat_cpp = mvstack.pop();

		// Thân vòi 2.2
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-2.895, 0.14, -0.8));
		truc2();
		model_mat_cpp = mvstack.pop();

		// Thân vòi 2.3
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-2.85, 0.12, -0.8));
		truc3();
		model_mat_cpp = mvstack.pop();

		//Giọt nước 2
		if (dropWater) {
			mvstack.push(model_mat_cpp);
			model_mat_cpp = model_mat_cpp * translate(vec3(-2.85, waterPosY, -0.8));
			water();
			model_mat_cpp = mvstack.pop();
		}

		//Bồn rửa tay
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-2.83, -0.4, -1.2));
		manh_left_right();
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-2.83, -0.4, -0.8));
		manh_left_right();
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-2.82, -0.485, -1.0));
		manh_bot();
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-2.72, -0.4, -1.0));
		manh_front();
		model_mat_cpp = mvstack.pop();
	}
}

//-----------------Ve du quay-----------------
bool isRotating1 = true; // Mặc định là quay
namespace duquay {

	void veThanhdoc(float x, float y, float z, float sx, float sy, float sz) {
		mvstack.push(model_mat_cpp);


		mat4 instance = identity_mat4();
		instance = translate(vec3(x, y, z)) * translate(vec3(x / 2, 0, 0)) * rotate_x(90) * translate(vec3(-(x / 2), 0, 0)) * scale(vec3(sx, sy, sx));

		mat4 model_trunk = model_mat_cpp * instance;

		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_trunk.m);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model_mat_cpp = mvstack.pop();
	}

	void veThanhngang(float x, float y, float z, float sx, float sy, float sz, float rotate) {
		mvstack.push(model_mat_cpp);


		mat4 instance = identity_mat4();
		instance = translate(vec3(x, y, z)) * translate(vec3(x / 2, 0, 0)) * rotate_z(rotate) * translate(vec3(-(x / 2), 0, 0)) * scale(vec3(sx, sy, sz));

		mat4 model_trunk = model_mat_cpp * instance;

		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_trunk.m);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model_mat_cpp = mvstack.pop();
	}
	void veGhe(float x, float y, float z, float sx, float sy, float sz) {
		mvstack.push(model_mat_cpp);
		setInt("color", 12);
		model_mat_cpp = model_mat_cpp * translate(vec3(x, y, z)) * scale(vec3(sx, sy, sz));
		veThanhngang(0, -0.35, 0, 0.04, 0.4, 0.25, 0);
		veThanhngang(0.12, -0.47, 0, 0.04, 0.4, 0.2, 90);
		veThanhngang(0, 0, 0.08, 0.01, 0.45, 0.01, 0);
		veThanhngang(0, 0, -0.08, 0.01, 0.45, 0.01, 0);

		model_mat_cpp = mvstack.pop();
	}
	void veBatgiac(float x, float y, float z) {
		mvstack.push(model_mat_cpp);
		setInt("color", 11); // Màu gỗ
		model_mat_cpp = model_mat_cpp * translate(vec3(x, y, z));
		veThanhngang(0, 0, 0, 0.05, 1.5, 0.05, 90);
		veThanhngang(-1.1, -0.15, 0, 0.05, 1.5, 0.05, 90 - 135);
		veThanhngang(-1.8, -1.8, 0, 0.05, 1.5, 0.05, 0);
		veThanhngang(-0.685, -2.82, 0, 0.05, 1.5, 0.05, 90 + 135);
		veThanhngang(0, -3.575, 0, 0.05, 1.5, 0.05, 90);
		veThanhngang(0.675, -0.775, 0, 0.05, 1.5, 0.05, 90 + 135);
		veThanhngang(1.775, -1.8, 0, 0.05, 1.5, 0.05, 0);
		veThanhngang(1.105, -3.45, 0, 0.05, 1.5, 0.05, 90 - 135);


		model_mat_cpp = mvstack.pop();

	}
	void veTruc(float x, float y, float z, float sx, float sy, float sz) {
		mvstack.push(model_mat_cpp);
		setInt("color", 1); // Màu gỗ
		model_mat_cpp = model_mat_cpp * translate(vec3(x, y, z)) * scale(vec3(sx, sy, sz)) * rotate_z(45);
		veThanhdoc(0, 0, 0.2, 1, 1, 1);
		model_mat_cpp = mvstack.pop();
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(x, y, z)) * scale(vec3(sx, sy, sz));
		veThanhdoc(0, 0, 0.2, 1, 1, 1);
		model_mat_cpp = mvstack.pop();
	}
	void veConQuayTo(float x, float y, float z, float sx, float sy, float sz) {
		mvstack.push(model_mat_cpp);
		setInt("color", 11); // Màu gỗ
		model_mat_cpp = model_mat_cpp * translate(vec3(x, y, z)) * translate(vec3(0, 1.4, 0)) * scale(vec3(sx, sy, sz));
		veBatgiac(0, 0, 0);
		veBatgiac(0, 0, +0.5);
		// Các thanh gắn
		veThanhdoc(+0.73, 0, +0.25, 0.05, 0.5, +z / 2);
		veThanhdoc(-0.73, 0, +0.25, 0.05, 0.5, +z / 2);

		veThanhdoc(-0.73 * 2.5 + 0.025, -1.05, +0.25, 0.05, 0.5, z / 2);
		veThanhdoc(-0.73 * 2.5 + 0.025, -1.05 * 2.5 + 0.075, 0.25, 0.05, 0.5, z / 2);

		veThanhdoc(+0.73 * 2.5 - 0.05, -1.05, +0.25, 0.05, 0.5, z + z / 2);
		veThanhdoc(+0.73 * 2.5 - 0.05, -1.05 * 2.5 + 0.075, 0.25, 0.05, 0.5, z / 2);

		veThanhdoc(+0.73, -3.575, +0.25, 0.05, 0.5, z / 2);
		veThanhdoc(-0.73, -3.575, +0.25, 0.05, 0.5, z / 2);

		model_mat_cpp = mvstack.pop();

	}
	void veLucgiac(float x, float y, float z, float sx, float sy, float sz) {
		mvstack.push(model_mat_cpp);
		setInt("color", 11); // Màu gỗ
		model_mat_cpp = model_mat_cpp * translate(vec3(x, y, z)) * scale(vec3(sx, sy, sz));
		veThanhngang(0, 0, 0, 0.05, 0.5, 0.1, 90);

		veThanhngang(-0.18, -0.25, 0, 0.05, 0.5, 0.1, 90 + 60);
		veThanhngang(-0.18, -0.55, 0, 0.05, 0.5, 0.1, 90 + 60 + 60);

		veThanhngang(0, -0.8, 0, 0.05, 0.5, 0.1, 90);

		veThanhngang(0.33, -0.12, 0, 0.05, 0.5, 0.1, 90 - 60);
		veThanhngang(0.33, -0.67, 0, 0.05, 0.5, 0.1, 90 - 60 - 60);



		model_mat_cpp = mvstack.pop();

	}
	void veConQuayNho(float x, float y, float z, float sx, float sy, float sz) {
		mvstack.push(model_mat_cpp);
		setInt("color", 11); // Màu gỗ
		model_mat_cpp = model_mat_cpp * translate(vec3(x, y, z)) * scale(vec3(sx, sy, sz));
		veLucgiac(0, 0, 0, 1, 1, 1);
		veLucgiac(0, 0, 0.5, 1, 1, 1);

		veThanhdoc(0.23, 0, 0.25, 0.05, 0.5, +0.5);
		veThanhdoc(-0.23, 0, 0.25, 0.05, 0.5, +0.5);
		veThanhdoc(0.45, -0.4, 0.25, 0.05, 0.5, +0.5);
		veThanhdoc(-0.45, -0.4, 0.25, 0.05, 0.5, +0.5);
		veThanhdoc(+0.23, -0.8, 0.25, 0.05, 0.5, +0.5);
		veThanhdoc(-0.23, -0.8, 0.25, 0.05, 0.5, +0.5);

		veThanhngang(0.83, -0.3, 0, 0.05, 1.5, 0.05, -65);
		veThanhngang(0.83, -0.3, 0.5, 0.05, 1.5, 0.05, -65);

		veThanhngang(-0.86, -0.3, 0, 0.05, 1.5, 0.05, -65);
		veThanhngang(-0.86, -0.3, 0.5, 0.05, 1.5, 0.05, -65);

		veThanhngang(-0.35, 0.65, 0, 0.05, 1.5, 0.05, 20);
		veThanhngang(-0.35, 0.65, 0.5, 0.05, 1.5, 0.05, 20);

		veThanhngang(0.35, -1.45, 0, 0.05, 1.5, 0.05, 20);
		veThanhngang(0.35, -1.45, 0.5, 0.05, 1.5, 0.05, 20);
		model_mat_cpp = mvstack.pop();

	}


	void veConQuay(float x, float y, float z, float sx, float sy, float sz) {
		mvstack.push(model_mat_cpp);
		setInt("color", 11); // Màu gỗ

		// Tính thời gian đã trôi qua
		float elapsed_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Chuyển từ mili giây sang giây
		float speed_factor = 6.0f; // Thêm một hệ số tốc độ quay
		float angle = isRotating1 ? (elapsed_time / speed_factor) * 360.0f : 0.0f;

		// Đặt phép quay quanh tâm
		model_mat_cpp = model_mat_cpp * translate(vec3(x, y, z)) * translate(vec3(0, -1.2, 0)) * rotate_z(22) * rotate_z(angle) * translate(vec3(0, 1.2, 0)) * scale(vec3(sx, sy, sz));

		// Vẽ các phần của đu quay
		veConQuayNho(0, 0, 0, 1, 1, 1); // Vẽ con quay nhỏ
		veConQuayTo(0, 0, 0, 1, 1, 1);  // Vẽ con quay lớn

		// Khôi phục lại ma trận trước đó
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		// Tính toán thời gian và góc quay
		float elapsed_time1 = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Thời gian tính bằng giây
		float speed_factor1 = 6.0f; // Hệ số tốc độ quay
		float angle1 = isRotating1 ? (elapsed_time / speed_factor) * 360.0f : 0.0f; // Góc quay tính bằng độ
		float radians1 = angle1 * M_PI / 180.0f; // Chuyển đổi góc từ độ sang radian

		// Bán kính quỹ đạo của ghế quanh tâm
		float radius = 1.95f; // Khoảng cách từ tâm ConQuayTo tới vị trí ghế
		model_mat_cpp = model_mat_cpp * translate(vec3(x, y, z)) * translate(vec3(0, -1.8, 0)) * scale(vec3(sx, sy, sz));
		// Vẽ 8 ghế
		for (int i = 0; i < 8; i++) {
			// Tính góc của ghế hiện tại
			float current_angle1 = radians1 + (i * 45.0f * M_PI / 180.0f); // Cộng thêm góc lệch của ghế (đổi 45 độ sang radian)

			// Tính tọa độ ghế theo quỹ đạo tròn
			float ghe_x = radius * std::cos(current_angle1); // Tịnh tiến x theo góc quay
			float ghe_y = radius * std::sin(current_angle1); // Tịnh tiến y theo góc quay
			float ghe_z = 0.2f;                            // Giữ nguyên độ cao (z)

			// Vẽ từng ghế
			mvstack.push(model_mat_cpp); // Lưu trạng thái trước khi vẽ ghế
			model_mat_cpp = model_mat_cpp * translate(vec3(ghe_x, ghe_y, ghe_z)); // Tịnh tiến ghế theo vị trí đã tính
			veGhe(0, 0, 0, 1, 1, 1); // Vẽ ghế
			model_mat_cpp = mvstack.pop();
		}

		// Khôi phục trạng thái ban đầu sau khi hoàn thành
		model_mat_cpp = mvstack.pop();
	}
	// Hàm vẽ đế (hình trụ lớn hoặc hình nón)
	void veDe(float x, float y, float z, float sx, float sy, float sz) {
		// Lưu trạng thái ma trận ban đầu
		mvstack.push(model_mat_cpp);

		setInt("color", 13);  // Đặt màu cho giá đỡ bánh xe
		model_mat_cpp = model_mat_cpp * translate(vec3(x, y, z)) * scale(vec3(sx, sy, sz));
		veThanhngang(0.8, 0.1, -0.2, 0.1, 3, 0.1, 25);
		veThanhngang(-0.8, 0.1, -0.2, 0.1, 3, 0.1, -25);

		veThanhngang(0.8, 0.1, 0.7, 0.1, 3, 0.1, 25);
		veThanhngang(-0.8, 0.1, 0.7, 0.1, 3, 0.1, -25);

		model_mat_cpp = mvstack.pop();
	}


	void veDuQuay(float x, float y, float z, float sx, float sy, float sz) {
		veConQuay(x, y, z, sx, sy, sz);
		veDe(x, y - 5.5, z, sx, sy + 0.5, sz);
		veTruc(x, y - 1.25, z, sx - 1.5, sy - 1.5, sz + 0.5);
	}


}

//-----------------------------Thùng giác----------------------------------
bool mo_thungrac = false;
void veThungrac(float x, float y, float z, float sx, float sy, float sz) {
	// vẽ phần thân
	mvstack.push(model_mat_cpp);
	setInt("color", 13);
	mat4 instance = identity_mat4();
	instance = translate(vec3(x, y, z)) * scale(vec3(sx, sy, sz)) * rotate_x(90.0f);
	mat4 model = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model.m);
	glDrawArrays(GL_TRIANGLES, 0, 32);
	model_mat_cpp = mvstack.pop();
	// vẽ nắp thùng rác
	mvstack.push(model_mat_cpp);
	setInt("color", 15);
	mat4 instance1 = identity_mat4();
	if (mo_thungrac == false) {
		instance1 = translate(vec3(x, y + 0.35f, z)) * scale(vec3(0.55f, 0.05f, 0.55f));
	}
	else {
		instance1 = translate(vec3(x, y + 0.35f, z)) * translate(vec3(0, 0, 0.275f)) * rotate_x(90.0f) * translate(vec3(0, 0, -0.275f)) * scale(vec3(0.55f, 0.05f, 0.55f));
	}
	mat4 model1 = model_mat_cpp * instance1;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model1.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model_mat_cpp = mvstack.pop();
}

// ------------------------------------ Thông số cho cây ------------------------------------
#define chieucao_than 2.5     // Chiều cao của thân cây
#define duong_kinh_than 0.3    // Đường kính của thân cây
#define chieucao_la 1.7f       // Chiều cao của phần lá (tán cây)
#define duong_kinh_la 2.5f     // Đường kính lớn nhất của phần lá
namespace cay {
	// Hàm vẽ thân cây (hình trụ)
	void vethancay(float rotate) {
		mvstack.push(model_mat_cpp);
		setInt("color", 10); // Màu gỗ

		mat4 instance = identity_mat4();
		instance = translate(vec3(0.0, chieucao_than / 2.0, 0.0)) *
			scale(vec3(duong_kinh_than, chieucao_than, duong_kinh_than)) *
			rotate_y(rotate); // Quay thân cây theo trục Y

		mat4 model_trunk = model_mat_cpp * instance;

		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_trunk.m);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model_mat_cpp = mvstack.pop();
	}

	// Hàm vẽ tán lá (tạo hiệu ứng nhiều lớp và ngẫu nhiên)
	void velacay(float sway_angle) {
		mvstack.push(model_mat_cpp);
		setInt("color", 5); // Màu xanh lá cây

		for (int i = 0; i < 3; ++i) {
			float scale_factor = 1.0f - 0.3f * i; // Lớp tán nhỏ dần
			float height_offset = i * chieucao_la * 0.5f; // Tán lá cao hơn mỗi lớp

			mat4 instance = identity_mat4();
			instance = translate(vec3(0.0, chieucao_than + height_offset, 0.0)) *
				rotate_y(sway_angle) * // Hiệu ứng đung đưa
				scale(vec3(duong_kinh_la * scale_factor, chieucao_la * scale_factor, duong_kinh_la * scale_factor));

			mat4 model_leaves = model_mat_cpp * instance;

			glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_leaves.m);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		model_mat_cpp = mvstack.pop();
	}
	void velacay2(float sway_angle) {
		mvstack.push(model_mat_cpp);
		setInt("color", 5); // Màu xanh lá cây

		for (int i = 0; i < 3; ++i) {
			float scale_factor = 1.0f - 0.3f * i; // Lớp tán nhỏ dần
			float height_offset = i * chieucao_la * 0.5f; // Tán lá cao hơn mỗi lớp

			mat4 instance = identity_mat4();
			instance = translate(vec3(0.0, chieucao_than + height_offset, 0.0)) *
				rotate_y(sway_angle) * // Hiệu ứng đung đưa
				scale(vec3(duong_kinh_la * scale_factor, chieucao_la * scale_factor, duong_kinh_la * scale_factor)) *
				rotate_y(45);

			mat4 model_leaves = model_mat_cpp * instance;

			glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_leaves.m);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		model_mat_cpp = mvstack.pop();
	}
	// Hàm vẽ cây với đung đưa
	void vecay(float tx, float ty, float tz, float sx, float sy, float sz) {
		mvstack.push(model_mat_cpp);

		// Dịch chuyển và thay đổi kích thước toàn bộ cây
		model_mat_cpp = model_mat_cpp * translate(vec3(tx, -1.15, tz)) * scale(vec3(sx, sy, sz));

		// Tính góc đung đưa
		float elapsed_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Chuyển mili giây sang giây
		float sway_angle = 10.0f * sin(elapsed_time); // Biên độ 5 độ, thay đổi theo thời gian

		// Vẽ thân cây
		cay::vethancay(0);
		cay::vethancay(45);
		cay::vethancay(90);

		// Dịch chuyển để vẽ tán lá
		mvstack.push(model_mat_cpp);
		cay::velacay(sway_angle);
		cay::velacay2(sway_angle);
		model_mat_cpp = mvstack.pop();

		model_mat_cpp = mvstack.pop();
	}
}

void cube1(vec3 t, vec3 s)
{
	mat4 instance = identity_mat4();
	instance = translate(t) * scale(s);
	mat4 model_cube = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_cube.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void pillar()
{
	mat4 instance = identity_mat4();
	instance = scale(vec3(0.2, 5, 0.2));
	mat4 model_cube = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_cube.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void floor()
{
	mat4 instance = identity_mat4();
	instance = scale(vec3(2, 0.1, 2));
	mat4 model_cube = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_cube.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void floor1()
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(0, -0.1, 0)) * scale(vec3(1.5, 0.1, 1.5));
	mat4 model_cube = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_cube.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void floor2()
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(0, -0.2, 0)) * scale(vec3(1, 0.1, 1));
	mat4 model_cube = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_cube.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void celling()
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(0, 0.5 + 0.05, 0)) * scale(vec3(2, 0.1, 2));
	mat4 model_cube = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_cube.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void celling1()
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(0, 0.5 + 0.05 + 0.1, 0)) * scale(vec3(1.5, 0.1, 1.5));
	mat4 model_cube = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_cube.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void celling2()
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(0, 0.5 + 0.05 + 0.1 * 2, 0)) * scale(vec3(1, 0.1, 1));
	mat4 model_cube = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_cube.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void box(vec3 t)
{
	cube1(vec3(0 + t.x, t.y + 0, t.z), vec3(0.5, 0.5, 0.1));
	cube1(vec3(t.x + 0.5 / 2 - 0.1 / 2, t.y + 0, t.z + 0.5 / 2), vec3(0.1, 0.5, 0.5));
	cube1(vec3(t.x + -(0.5 / 2 - 0.1 / 2), t.y + 0, t.z + 0.5 / 2), vec3(0.1, 0.5, 0.5));
	cube1(vec3(t.x + (0.5 / 2 - 0.1 / 2) - 0.1, t.y + -0.5 / 2 + 0.2 / 2, t.z + 0.5 / 2), vec3(0.2, 0.2, 0.5));
}
void cua(vec3 t)
{
	cube1(vec3(t.x + -(0.5 / 2 - 0.1 / 2), t.y + 0, t.z + 0.5 / 2 + 0.1 * 2), vec3(0.1, 0.5, 0.1));
	cube1(vec3(t.x + 0.5 / 2 + 0.1 / 2 - 0.1, t.y + 0, t.z + 0.5 / 2 + 0.1 * 2), vec3(0.1, 0.5, 0.1));
	cube1(vec3(t.x + 0, t.y + 0, t.z + 0.5 - 0.1 / 2), vec3(0.5, 0.1, 0.1));
	cube1(vec3(t.x + 0, t.y + 0.5 / 2 - 0.1 / 2, t.z + 0.5 - 0.1 / 2), vec3(0.5, 0.1, 0.1));
	cube1(vec3(t.x + 0, t.y + -(0.5 / 2 - 0.1 / 2), t.z + 0.5 - 0.1 / 2), vec3(0.5, 0.1, 0.1));
}
void cua1(vec3 t)
{
	cube1(vec3(t.x + -(0.5 / 2 - 0.1 / 2), t.y + 0, t.z + 0.5 / 2 + 0.1 / 2 - 0.5 / 2), vec3(0.1, 0.5, 0.1));
	cube1(vec3(t.x + (0.5 / 2 - 0.1 / 2), t.y + 0, t.z + 0.5 / 2 + 0.1 / 2 - 0.5 / 2), vec3(0.1, 0.5, 0.1));
	cube1(vec3(t.x + 0, t.y + 0, t.z + 0 + 0.1 / 2), vec3(0.5, 0.1, 0.1));
	cube1(vec3(t.x + 0, t.y + 0.5 / 2 - 0.1 / 2, t.z + 0 + 0.1 / 2), vec3(0.5, 0.1, 0.1));
	cube1(vec3(t.x + 0, t.y + -(0.5 / 2 - 0.1 / 2), t.z + 0 + 0.1 / 2), vec3(0.5, 0.1, 0.1));
}
void cua2(vec3 t)
{
	cube1(vec3(t.x + -(0.5 / 2 - 0.1 / 2), t.y + 0, t.z + 0.5 / 2 + 0.5 / 2 - 0.05), vec3(0.1, 0.5, 0.1));
	cube1(vec3(t.x + -(0.5 / 2 - 0.1 / 2), t.y + 0, t.z), vec3(0.1, 0.5, 0.1));
	cube1(vec3(t.x + -(0.5 / 2 - 0.1 / 2), t.y + 0, t.z + 0.5 / 2), vec3(0.1, 0.1, 0.5));
	cube1(vec3(t.x + -(0.5 / 2 - 0.1 / 2), t.y + 0.5 / 2 - 0.1 / 2, t.z + 0.5 / 2), vec3(0.1, 0.1, 0.5));
	cube1(vec3(t.x + -(0.5 / 2 - 0.1 / 2), t.y + -(0.5 / 2 - 0.1 / 2), t.z + 0.5 / 2), vec3(0.1, 0.1, 0.5));
}
void cua3(vec3 t)
{
	cube1(vec3(t.x + (0.5 / 2 - 0.1 / 2), t.y + 0, t.z + 0.5 / 2 + 0.5 / 2 - 0.05), vec3(0.1, 0.5, 0.1));
	cube1(vec3(t.x + (0.5 / 2 - 0.1 / 2), t.y + 0, t.z), vec3(0.1, 0.5, 0.1));
	cube1(vec3(t.x + 0.5 / 2 - 0.1 / 2, t.y + 0, t.z + 0.5 / 2), vec3(0.1, 0.1, 0.5));
	cube1(vec3(t.x + 0.5 / 2 - 0.1 / 2, t.y + 0.5 / 2 - 0.1 / 2, t.z + 0.5 / 2), vec3(0.1, 0.1, 0.5));
	cube1(vec3(t.x + 0.5 / 2 - 0.1 / 2, t.y + -(0.5 / 2 - 0.1 / 2), t.z + 0.5 / 2), vec3(0.1, 0.1, 0.5));
}
void box1(vec3 t)
{
	cube1(vec3(t.x + 0, t.y + 0, t.z + 0.5 - 0.1 / 2), vec3(0.5, 0.5, 0.1));
	cube1(vec3(t.x + 0.5 / 2 - 0.1 / 2, t.y + 0, t.z + 0.5 / 2), vec3(0.1, 0.5, 0.5));
	cube1(vec3(t.x + -(0.5 / 2 - 0.1 / 2), t.y + 0, t.z + 0.5 / 2), vec3(0.1, 0.5, 0.5));
	cube1(vec3(t.x + -((0.5 / 2 - 0.1 / 2) - 0.1), t.y + -0.5 / 2 + 0.2 / 2, t.z + 0.5 / 2), vec3(0.2, 0.2, 0.5));
}
void box2(vec3 t)
{
	cube1(vec3(t.x + 0, t.y + 0, t.z + 0.5 - 0.1 / 2), vec3(0.5, 0.5, 0.1));
	cube1(vec3(t.x + 0.5 / 2 - 0.1 / 2, t.y + 0, t.z + 0.5 / 2), vec3(0.1, 0.5, 0.5));
	cube1(vec3(t.x + 0, t.y + -0.5 / 2 + 0.2 / 2, t.z + ((0.5 / 2 - 0.1 / 2) + 0.1)), vec3(0.5, 0.2, 0.2));
	cube1(vec3(t.x + 0, t.y + 0, t.z + 0), vec3(0.5, 0.5, 0.1));
}
void box3(vec3 t)
{
	cube1(vec3(t.x + 0, t.y + 0, t.z + 0.5 - 0.1 / 2), vec3(0.5, 0.5, 0.1));
	cube1(vec3(t.x + -(0.5 / 2 - 0.1 / 2), t.y + 0, t.z + 0.5 / 2), vec3(0.1, 0.5, 0.5));
	cube1(vec3(t.x + 0, t.y + -0.5 / 2 + 0.2 / 2, t.z + 0.2 / 2 + 0.1 / 2), vec3(0.5, 0.2, 0.2));
	cube1(vec3(t.x + 0, t.y + 0, t.z + 0), vec3(0.5, 0.5, 0.1));
}
void table0()
{
	mat4 instance = identity_mat4();
	instance = scale(vec3(0.5, 0.1, 0.5));
	mat4 model_table0 = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_table0.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void table1()
{
	mat4 instance = identity_mat4();
	instance = translate(vec3((-0.5 / 2 + 0.05 / 2), (-0.3 / 2), (0.25 - 0.05 / 2))) * scale(vec3(0.05, 0.3, 0.05));
	mat4 model_table0 = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_table0.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void table2()
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(-(-0.5 / 2 + 0.05 / 2), (-0.3 / 2), (0.25 - 0.05 / 2))) * scale(vec3(0.05, 0.3, 0.05));
	mat4 model_table0 = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_table0.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void table3()
{
	mat4 instance = identity_mat4();
	instance = translate(vec3((-0.5 / 2 + 0.05 / 2), (-0.3 / 2), -(0.25 - 0.05 / 2))) * scale(vec3(0.05, 0.3, 0.05));
	mat4 model_table0 = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_table0.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void table4()
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(-(-0.5 / 2 + 0.05 / 2), (-0.3 / 2), -(0.25 - 0.05 / 2))) * scale(vec3(0.05, 0.3, 0.05));
	mat4 model_table0 = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_table0.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void chair0(float offset_x, float offset_y)
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(offset_x + 0.5, offset_y + 0, 0)) * scale(vec3(0.25, 0.05, 0.25));
	mat4 model_table0 = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_table0.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void chair1(float offset_x, float offset_y)
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(offset_x + 0.5 + 0.25 / 2 - 0.05 / 2, offset_y + -(0.2 / 2 + 0.05 / 2), 0.25 / 2 - 0.05 / 2)) * scale(vec3(0.05, 0.2, 0.05));
	mat4 model_table0 = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_table0.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void chair2(float offset_x, float offset_y)
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(offset_x + 0.5 - (0.25 / 2 - 0.05 / 2), offset_y + -(0.2 / 2 + 0.05 / 2), 0.25 / 2 - 0.05 / 2)) * scale(vec3(0.05, 0.2, 0.05));
	mat4 model_table0 = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_table0.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void chair3(float offset_x, float offset_y)
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(offset_x + 0.5 + 0.25 / 2 - 0.05 / 2, offset_y + -(0.2 / 2 + 0.05 / 2), -(0.25 / 2 - 0.05 / 2))) * scale(vec3(0.05, 0.2, 0.05));
	mat4 model_table0 = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_table0.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void chair4(float offset_x, float offset_y)
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(offset_x + 0.5 - (0.25 / 2 - 0.05 / 2), offset_y + -(0.2 / 2 + 0.05 / 2), -(0.25 / 2 - 0.05 / 2))) * scale(vec3(0.05, 0.2, 0.05));
	mat4 model_table0 = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_table0.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
//
void chair5(float offset_x, float offset_y)
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(offset_x + 0.5 + 0.25 / 2 - 0.05 / 2, offset_y + (0.2 / 2 + 0.05 / 2), 0.25 / 2 - 0.05 / 2)) * scale(vec3(0.05, 0.2, 0.05));
	mat4 model_table0 = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_table0.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void chair6(float offset_x, float offset_y)
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(offset_x + 0.5 + 0.25 / 2 - 0.05 / 2, offset_y + (0.2 / 2 + 0.05 / 2), -(0.25 / 2 - 0.05 / 2))) * scale(vec3(0.05, 0.2, 0.05));
	mat4 model_table0 = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_table0.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void chair7(float offset_x, float offset_y)
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(offset_x + 0.5 + 0.25 / 2 - 0.05 / 2, offset_y + (0.2 / 2 + 0.05 / 2), 0)) * scale(vec3(0.05, 0.2, 0.05));
	mat4 model_table0 = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_table0.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void chair8(float offset_x, float offset_y)
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(offset_x + 0.5 + 0.25 / 2 - 0.05 / 2, offset_y + 0.075 + (0.2 / 2 + 0.05 / 2), 0)) * scale(vec3(0.05, 0.25 / 2, 0.35));
	mat4 model_table0 = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_table0.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void a_chair1(float offset_x, float offset_y)
{
	//
	chair0(offset_x, offset_y);
	chair1(offset_x, offset_y);
	chair2(offset_x, offset_y);
	chair3(offset_x, offset_y);
	chair4(offset_x, offset_y);

	//
	chair5(offset_x, offset_y);
	chair6(offset_x, offset_y);
	chair7(offset_x, offset_y);
	//
	chair8(offset_x, offset_y);
	////////////////////////////////
}
void a_chair2(float offset_x, float offset_y)
{
	//
	chair0(offset_x, offset_y);
	chair1(offset_x, offset_y);
	chair2(offset_x, offset_y);
	chair3(offset_x, offset_y);
	chair4(offset_x, offset_y);

	//
	chair5(offset_x - 0.2, offset_y);
	chair6(offset_x - 0.2, offset_y);
	chair7(offset_x - 0.2, offset_y);
	//
	chair8(offset_x - 0.2, offset_y);
	////////////////////////////////
}
// --------------------------------------------------------------------------
void chongchong0()
{
	mat4 instance = identity_mat4();
	instance = scale(vec3(0.05, 0.4, 0.05));
	mat4 model_chongchong = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_chongchong.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void chongchong1()
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(0, 0.4 / 2 - 0.05 / 2, 0.2 / 2 + 0.05 / 2)) * scale(vec3(0.05, 0.05, 0.2));
	mat4 model_chongchong = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_chongchong.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void chongchong2()
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(0, 0.4 / 2 - 0.05 / 2, 2 * (0.2 / 2 + 0.05 / 2))) * rotate_z(120) * translate(vec3(0, 0.25 / 2, 0)) * scale(vec3(0.05, 0.25, 0.05));
	mat4 model_chongchong = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_chongchong.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void chongchong3()
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(0, 0.4 / 2 - 0.05 / 2, 2 * (0.2 / 2 + 0.05 / 2))) * rotate_z(240) * translate(vec3(0, 0.25 / 2, 0)) * scale(vec3(0.05, 0.25, 0.05));
	mat4 model_chongchong = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_chongchong.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void chongchong4()
{
	mat4 instance = identity_mat4();
	instance = translate(vec3(0, 0.4 / 2 - 0.05 / 2, 2 * (0.2 / 2 + 0.05 / 2))) * rotate_z(0) * translate(vec3(0, 0.25 / 2, 0)) * scale(vec3(0.05, 0.25, 0.05));
	mat4 model_chongchong = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_chongchong.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
float quay = 0.0f;
void chongchong_full(vec3 tr)
{
	model_mat_cpp = model_mat_cpp * translate(tr) * rotate_y(180);
	chongchong0();
	chongchong1();

	model_mat_cpp = model_mat_cpp * translate(vec3(0, 0.4 / 2 - 0.05 / 2, 2 * (0.2 / 2 + 0.05 / 2))) * rotate_z(quay) * translate(vec3(0, -(0.4 / 2 - 0.05 / 2), -(2 * (0.2 / 2 + 0.05 / 2))));
	chongchong2();
	chongchong3();
	chongchong4();
}

void table_full(vec3 t)
{
	model_mat_cpp = model_mat_cpp * translate(t) * scale(vec3(2, 2, 2));
	table0();
	table1();
	table2();
	table3();
	table4();
	a_chair1(0.1, -0.075);
	a_chair2(-1.1, -0.075);
}
void vexichduthang(vec3(tr))
{

	model_mat_cpp = model_mat_cpp * translate(tr) * scale(vec3(3, 3, 3));
	pillar();

	model_mat_cpp = model_mat_cpp * translate(vec3(0, t, 0)) * rotate_y(z);

	floor();
	floor1();
	floor2();
	celling();
	celling1();
	celling2();

	box(vec3(0, 0.5 / 2 + 0.05, 0.4));
	box1(vec3(0, 0.5 / 2 + 0.05, -0.4 - 0.5));
	box2(vec3(-0.1 - 0.5, 0.5 / 2 + 0.05, -0.2));
	box3(vec3(0.1 + 0.5, 0.5 / 2 + 0.05, -0.2));

	mat4 temp1 = model_mat_cpp;
	model_mat_cpp = temp1 * translate(vec3(c, 0, 0));
	cua(vec3(0, 0.5 / 2 + 0.05, 0.4));

	model_mat_cpp = temp1 * translate(vec3(-c, 0, 0));
	cua1(vec3(0, 0.5 / 2 + 0.05, -0.4 - 0.5));

	model_mat_cpp = temp1 * translate(vec3(0, 0, c));
	cua2(vec3(-0.1 - 0.5, 0.5 / 2 + 0.05, -0.2));

	model_mat_cpp = temp1 * translate(vec3(0, 0, -c));
	cua3(vec3(0.1 + 0.5, 0.5 / 2 + 0.05, -0.2));
}


//---------------------------------Đèn đường------------------------------------
bool moden = false;
#define MAX_LIGHTS 10
vec4 lightPositions[MAX_LIGHTS];
int numLights = 0; // Số đèn hiện có

void denduong(float x, float y, float z, float sx, float sy, float sz, float angle) {
	// Áp dụng xoay tổng thể
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(x, y, z)) * rotate_y(angle); // Xoay cả đèn quanh trục Y tại vị trí x, y, z

	// Phần thân
	mvstack.push(model_mat_cpp);
	setInt("color", 1);
	mat4 instance = identity_mat4();
	instance = scale(vec3(sx, sy * 1.2, sz));
	mat4 model_post = model_mat_cpp * instance;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_post.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model_mat_cpp = mvstack.pop();

	// Tay đèn ngang
	mvstack.push(model_mat_cpp);
	setInt("color", 1);
	mat4 instance1 = identity_mat4();
	instance1 = translate(vec3(-0.85f, 3.0f, 0)) * rotate_z(90.0f) * scale(vec3(sx, sy / 2, sz));
	mat4 model_post1 = model_mat_cpp * instance1;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_post1.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model_mat_cpp = mvstack.pop();

	// Tay đèn xiên
	mvstack.push(model_mat_cpp);
	setInt("color", 1);
	mat4 instance2 = identity_mat4();
	instance2 = translate(vec3(-0.55f, 2.5f, 0)) * rotate_z(45.0f) * scale(vec3(sx, 1.6f, sz / 2));
	mat4 model_post2 = model_mat_cpp * instance2;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_post2.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model_mat_cpp = mvstack.pop();

	// Đầu nối
	mvstack.push(model_mat_cpp);
	setInt("color", 1);
	mat4 instance3 = identity_mat4();
	instance3 = translate(vec3(-1.7f, 2.7f, 0)) * scale(vec3(0.05f, 0.8f, 0.05f));
	mat4 model_post3 = model_mat_cpp * instance3;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_post3.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model_mat_cpp = mvstack.pop();

	// Phần đèn
	mvstack.push(model_mat_cpp);
	setInt("color", 15);
	mat4 than = identity_mat4();
	than = translate(vec3(-1.7f, 2.2f, 0)) * scale(vec3(0.45f, 0.45f, 0.45f));
	mat4 model_than = model_mat_cpp * than;
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_than.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Tính toán vị trí ánh sáng
	if (numLights < MAX_LIGHTS) {
		vec4 finalPosition = model_than * vec4(0, 0, 0, 1); // Vị trí sau phép biến đổi
		lightPositions[numLights] = finalPosition;
		numLights++;
	}

	// Cài đặt ánh sáng
	glUniform1i(glGetUniformLocation(ProgramId, "enable_light_3"), moden);
	glUniform3f(glGetUniformLocation(ProgramId, "light_color_custom"), 0.94, 0.9, 0.55); // Màu ánh sáng vàng nhạt
	glUniform4fv(glGetUniformLocation(ProgramId, "light_position_custom"), numLights, &lightPositions[0].x);
	glUniform1i(glGetUniformLocation(ProgramId, "soden"), numLights); // Truyền số lượng đèn

	model_mat_cpp = mvstack.pop(); // Phục hồi lại ma trận gốc
	model_mat_cpp = mvstack.pop();
}
// Ve hang rao 
namespace hangrao {
	void veCot(float x, float y, float z, float width, float height, float depth) {
		mvstack.push(model_mat_cpp);
		// Màu sắc cột hàng rào (màu gỗ)
		setInt("color", 10);  // Giả sử màu số 10 là màu gỗ

		mat4 instance = identity_mat4();
		// Dịch chuyển tới vị trí cần vẽ
		instance = translate(vec3(x, y, z)) * scale(vec3(width, height, depth));

		mat4 model_column = model_mat_cpp * instance;
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_column.m);

		// Vẽ cột (giả sử sử dụng một hình hộp)
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model_mat_cpp = mvstack.pop();
	}

	void veThanhNgang(float x, float y, float z, float width, float height, float depth) {
		mvstack.push(model_mat_cpp);

		// Màu sắc thanh ngang (màu gỗ)
		setInt("color", 10);

		mat4 instance = identity_mat4();
		// Dịch chuyển tới vị trí cần vẽ
		instance = translate(vec3(x, y, z)) * scale(vec3(width, height, depth));

		mat4 model_bar = model_mat_cpp * instance;
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_bar.m);

		// Vẽ thanh ngang (giả sử sử dụng hình hộp)
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model_mat_cpp = mvstack.pop();
	}

	void veHangRao(float startX, float startY, float startZ, float spacing, int numColumns,
		float columnWidth, float columnHeight, float columnDepth,
		float barWidth, float barHeight, float barDepth, float angle) {
		mvstack.push(model_mat_cpp);

		// Thêm xoay tổng quát cho hàng rào
		model_mat_cpp = model_mat_cpp * translate(vec3(startX, startY, startZ)) * rotate_y(angle);

		// Vẽ các cột
		for (int i = 0; i < numColumns; ++i) {
			float x = i * spacing; // Khoảng cách giữa các cột
			veCot(x, 0, 0, columnWidth, columnHeight, columnDepth); // Không cần startX, startY vì đã dịch tổng thể
		}

		// Vẽ thanh ngang nối các cột
		for (int i = 0; i < numColumns - 1; ++i) {
			float x = i * spacing + spacing / 2.0f;
			veThanhNgang(x, columnHeight / 2.0f, 0, barWidth, barHeight, barDepth);
			veThanhNgang(x, -columnHeight / 2.0f, 0, barWidth, barHeight, barDepth);
		}

		model_mat_cpp = mvstack.pop(); // Phục hồi ma trận gốc
	}
}
bool daynight = false;

const int numPoints = 100; // Số điểm trên đường ray
vec3 trackPoints[numPoints]; // Mảng lưu trữ các điểm
float trackPosition = 0.0f;
float trackSpeed = 0.03f;
int currentTrackPoint = 0; // Điểm hiện tại trên đường ray

void generateTrack() {
	for (int i = 0; i < numPoints; ++i) {
		float angle = 2.0f * M_PI * i / numPoints;
		float radius = 5.0f + 2.0f * sin(4.0f * angle); // Thay đổi bán kính để tạo hình dạng phức tạp hơn
		float height = 0.7f * sin(4.0f * angle); // Thay đổi chiều cao

		trackPoints[i].x = radius * cos(angle);
		trackPoints[i].y = height;
		trackPoints[i].z = radius * sin(angle) - 10.0f; // Dịch chuyển đường ray ra xa camera
	}
}

void drawTrackSupports() {
	setInt("color", 3); // Màu xám cho cột chống
	for (int i = 0; i < numPoints; i += 5) { // Giảm số cột bằng cách tăng khoảng cách giữa các cột
		mvstack.push(model_mat_cpp);
		vec3 base = vec3(trackPoints[i].x, -1.0f, trackPoints[i].z); // Chân cột chạm mặt đất (-1.0f)
		vec3 top = trackPoints[i]; // Đỉnh cột tại vị trí đường ray
		vec3 direction = top - base;

		float length = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
		float angleY = atan2(direction.x, direction.z);
		float angleX = -asin(direction.y / length);

		model_mat_cpp = model_mat_cpp * translate((base + top) / 2.0f) * rotate_y(angleY * 180.0f / M_PI) * rotate_x(angleX * 180.0f / M_PI) * scale(vec3(0.1f, 0.1f, length));
		cube();
		model_mat_cpp = mvstack.pop();
	}
}

void drawTrack() {
	setInt("color", 98); // Màu nâu cho đường ray
	for (int i = 0; i < numPoints - 1; ++i) {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(trackPoints[i]) * scale(vec3(0.2, 0.1, 0.5));
		cube();
		model_mat_cpp = mvstack.pop();

		// Kết nối các điểm
		mvstack.push(model_mat_cpp);
		vec3 midPoint = (trackPoints[i] + trackPoints[i + 1]) / 2.0f;
		vec3 direction = trackPoints[i + 1] - trackPoints[i];
		float length = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
		float angleY = atan2(direction.x, direction.z);
		float angleX = -asin(direction.y / length);

		model_mat_cpp = model_mat_cpp * translate(midPoint) * rotate_y(angleY * 180.0f / M_PI) * rotate_x(angleX * 180.0f / M_PI) * scale(vec3(0.1, 0.05, length));
		cube();
		model_mat_cpp = mvstack.pop();
	}

	// Kết nối điểm cuối và điểm đầu
	mvstack.push(model_mat_cpp);
	vec3 midPoint = (trackPoints[numPoints - 1] + trackPoints[0]) / 2.0f;
	vec3 direction = trackPoints[0] - trackPoints[numPoints - 1];
	float length = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
	float angleY = atan2(direction.x, direction.z);
	float angleX = -asin(direction.y / length);

	model_mat_cpp = model_mat_cpp * translate(midPoint) * rotate_y(angleY * 180.0f / M_PI) * rotate_x(angleX * 180.0f / M_PI) * scale(vec3(0.1, 0.05, length));
	cube();
	model_mat_cpp = mvstack.pop();
}

// Hàm vẽ thân tàu
void drawRollerCoasterBody() {
	setInt("color", 3); // Màu đỏ
	vec3 bodyScale = vec3(0.5, 0.3, 1.5);
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * scale(bodyScale);
	cube();
	model_mat_cpp = mvstack.pop();

	setInt("color", 10); // Màu vàng
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(0.0, 0.0, bodyScale.z / 2.0 + 0.2)) * scale(vec3(bodyScale.x, bodyScale.y, 0.4));
	cube();
	model_mat_cpp = mvstack.pop();

	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(0.0, 0.2, -bodyScale.z / 2.0)) * scale(vec3(0.4, 0.2, 0.05));
	cube();
	model_mat_cpp = mvstack.pop();

	// dau tau
	setInt("color", 7);
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(0.0, 0.17, 1.1)) * rotate_x(-20) * scale(vec3(0.5, 0.65, 0.15));
	cube();
	model_mat_cpp = mvstack.pop();
}

// Hàm vẽ ghế ngồi
void drawRollerCoasterSeats() {
	setInt("color", 4);
	vec3 seatScale = vec3(0.2, 0.1, 0.2);
	float seatSpacing = 0.4;
	int numSeats = 4;

	for (int i = 0; i < numSeats; ++i) {
		float seatZ = -0.4 + i * seatSpacing;

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(-0.15, 0.2, seatZ)) * scale(seatScale);
		cube();
		model_mat_cpp = mvstack.pop();

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(0.15, 0.2, seatZ)) * scale(seatScale);
		cube();
		model_mat_cpp = mvstack.pop();
	}
}


// Hàm di chuyển tàu
void moveRollerCoaster() {
	trackPosition += trackSpeed;
	if (trackPosition >= 1.0f) {
		trackPosition -= 1.0f;
		currentTrackPoint = (currentTrackPoint + 1) % (numPoints - 1);
	}
}

void drawRollerCoaster() {
	moveRollerCoaster();

	vec3 currentPos = trackPoints[currentTrackPoint] * (1.0f - trackPosition) + trackPoints[currentTrackPoint + 1] * trackPosition;

	vec3 nextPos = trackPoints[(currentTrackPoint + 1) % (numPoints - 1)];
	if (trackPosition + trackSpeed < 1.0f) {
		nextPos = trackPoints[currentTrackPoint] * (1.0f - (trackPosition + trackSpeed)) + trackPoints[currentTrackPoint + 1] * (trackPosition + trackSpeed);
	}
	vec3 direction = normalize(nextPos - currentPos);

	float angleY = atan2(direction.x, direction.z) * 180.0f / M_PI;

	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(currentPos) * rotate_y(angleY);
	drawRollerCoasterBody();
	drawRollerCoasterSeats();
	model_mat_cpp = mvstack.pop();
}

// xích đu----------------
float boatSwingAngle = 0.0f;
float boatSwingSpeed = 0.05f;
float boatSwingMaxAngle = 22.0f;
bool boatSwingDirection = true;
void drawBoat() {
	//ghế trước
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(0.2, 0.0, 0.325)) * rotate_z(-10) * scale(vec3(0.7, 0.1, 0.8));
	setInt("color", 6); // Màu đỏ
	cube();
	model_mat_cpp = mvstack.pop();

	//mảnh sau gế
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(-0.2, 0.25, 0.325)) * rotate_z(-65) * scale(vec3(0.6, 0.1, 0.75));
	setInt("color", 6); // Màu đỏ
	cube();
	model_mat_cpp = mvstack.pop();
}

void drawBoatSwing() {
	// Vẽ các cột chống
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(2.0, 0.75, -8.0)) * scale(vec3(0.2, 3.5, 0.2));
	setInt("color", 2); // Màu xanh lá
	cube();
	model_mat_cpp = mvstack.pop();


	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(2.0, 0.75, -6.0)) * scale(vec3(0.2, 3.5, 0.2));
	setInt("color", 2); // Màu xanh lá
	cube();
	model_mat_cpp = mvstack.pop();

	// Vẽ thanh ngang
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(2.0, 2.5, -7.0)) * scale(vec3(0.2, 0.2, 2.0));
	setInt("color", 4); // Màu vàng
	cube();
	model_mat_cpp = mvstack.pop();
	mat4 boat_swing_mat = translate(vec3(2.0, 2.5, -7.0)) * rotate_z(boatSwingAngle) * translate(vec3(0.0, -1.5, 0.0));

	// Vẽ dây treo
	for (int i = -1; i <= 1; i += 2) {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(2.0, 2.8, -7.0 + i * 0.5)) * rotate_z(boatSwingAngle) * translate(vec3(0.0, -1.5, 0.0)) * scale(vec3(0.05, 2.4, 0.05));
		setInt("color", 12); // Màu đen
		cube();

		// Lưu lại ma trận model view sau khi vẽ dây
		mat4 model_mat_after_rope = model_mat_cpp;
		model_mat_cpp = mvstack.pop();

		// Vẽ thuyền dựa vào vị trí dây treo
		mvstack.push(model_mat_cpp);
		// Di chuyển đến điểm cuối của dây
		model_mat_cpp = model_mat_after_rope * translate(vec3(0.0, -1.5, 0.0)) * scale(vec3(20, 1 / 3.0, 20)) * rotate_y(180);
		model_mat_cpp = mvstack.pop();
	}
	// Chỉ vẽ 1 thuyền
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * boat_swing_mat * translate(vec3(0.0, -1.0, 0.5)) * rotate_y(180) * scale(vec3(1.5, 1.5, 1.5));
	drawBoat();
	model_mat_cpp = mvstack.pop();
}

void updateBoatSwing() {
	if (boatSwingDirection) {
		boatSwingAngle += boatSwingSpeed;
		if (boatSwingAngle > boatSwingMaxAngle) {
			boatSwingAngle = boatSwingMaxAngle;
			boatSwingDirection = false;
		}
	}
	else {
		boatSwingAngle -= boatSwingSpeed;
		if (boatSwingAngle < -boatSwingMaxAngle) {
			boatSwingAngle = -boatSwingMaxAngle;
			boatSwingDirection = true;
		}
	}
}

bool stageLightOn = true; // Global variable to control the stage lighting

void sankhau() {
	// Base
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(9.0f, -0.85f, 0.0f)) * scale(vec3(5.0f, 0.4f, 4.0f));
	setInt("color", 1);
	if (stageLightOn) {
		// Set uniforms for lighting (e.g., enable lighting, set light color, etc.)
		glUniform1i(glGetUniformLocation(ProgramId, "stage_lighting_enabled"), 1); // Example: Enable a specific light for the stage
		// Set other lighting-related uniforms as needed...
	}
	else {
		// Disable lighting for the stage (or set ambient-only lighting)
		glUniform1i(glGetUniformLocation(ProgramId, "stage_lighting_enabled"), 0);
		// ...
	}
	cube();
	model_mat_cpp = mvstack.pop();

	// Back
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(9.0f, 0.6f, 2.0f)) * scale(vec3(5.0f, 3.0f, 0.4f));
	setInt("color", 1);
	if (stageLightOn) {
		// Use the same lighting settings as the base
		glUniform1i(glGetUniformLocation(ProgramId, "stage_lighting_enabled"), 1);
		// ...
	}
	else {
		// Disable lighting (or set ambient-only lighting)
		glUniform1i(glGetUniformLocation(ProgramId, "stage_lighting_enabled"), 0);
		// ...
	}
	cube();
	model_mat_cpp = mvstack.pop();
}

void drawFlag() {
	// Pole (Cylinder)
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(-3.9f, 1.7f, -0.65f)) * scale(vec3(0.15f, 5.8f, 0.15f));
	setInt("color", 5); // Gray or metallic color for the pole
	cube();
	model_mat_cpp = mvstack.pop();

	// Flag (Rectangle)
	mvstack.push(model_mat_cpp);

	// Get the elapsed time in seconds
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	float waveAngle = 6.0f * sin(3.0f * time);

	model_mat_cpp = model_mat_cpp * translate(vec3(-2.85f, 3.6f, 0.0f)) * rotate_y(waveAngle - 30.0f) * scale(vec3(2.5f, 1.5f, 0.1f));
	setInt("color", 27); // Red color for the flag
	cube();
	model_mat_cpp = mvstack.pop();
}
void drawclock_right() {
	// lock right
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(-1.6f, 0.0f, -0.009f)) * scale(vec3(0.5f, 0.5f, 0.22f));
	setInt("color", 1); // Door Color
	cube();
	model_mat_cpp = mvstack.pop();

}

void drawclock_left() {

	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(-3.0f, 0.0f, -0.009f)) * scale(vec3(0.5f, 0.5f, 0.22f));
	setInt("color", 1); // Door Color
	cube();
	model_mat_cpp = mvstack.pop();
}

float k = 0.0f; // Khai báo k ở phạm vi global
void drawGate() {
	// --- Left Pillar ---
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(-13.6f, 0.5f, 0.0f)) * scale(vec3(0.5f, 3.8f, 0.5f));
	setInt("color", 3); // Example: Brown color for the pillars
	cube();
	model_mat_cpp = mvstack.pop();

	// --- Right Pillar ---
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(-9.0f, 0.5f, 0.0f)) * scale(vec3(0.5f, 3.8f, 0.5f));
	setInt("color", 3); // Same color as the left pillar
	cube();
	model_mat_cpp = mvstack.pop();

	// --- Arch ---
	model_mat_cpp = model_mat_cpp * translate(vec3(-9.0f, 1.0f, 0.0f));//di chuyển toàn bộ mô hình
	int numSegments = 15;
	float archRadius = 2.3f;
	float segmentWidth = 0.5f;
	float segmentHeight = 0.4f;
	float segmentDepth = 0.6f;

	for (int i = 0; i < numSegments; ++i) {
		float angle = (float)i / (numSegments - 1) * 3.14159f; // Angle from 0 to PI
		float x = cos(angle) * archRadius;
		float y = sin(angle) * archRadius;

		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(x - archRadius, y + 1.5f, 0.0f)) * rotate_y(90 - angle * 180 / 3.14159f) * scale(vec3(segmentWidth, segmentHeight, segmentDepth));
		setInt("color", 6); // Example: A stone-like color for the arch
		cube();
		model_mat_cpp = mvstack.pop();
	}


	float doorHeight = 3.7f;
	float doorWidth = 2.55f;
	float doorDepth = 0.2f;

	// --- Left Door Panel ---
	mvstack.push(model_mat_cpp);

	mat4 leftdoor = model_mat_cpp * translate(vec3(-5.0f + 0.25f, -2.4f, 0.0f)) *  // Di chuyển tâm xoay ra cạnh trụ trái
		rotate_y(-k) * translate(vec3(doorWidth / 2.0f, doorHeight / 2.0f, 0.0f));  // Di chuyển tâm xoay ra giữa cửa


	model_mat_cpp = leftdoor * scale(vec3(doorWidth, doorHeight, doorDepth));
	setInt("color", 27); // Door Color
	cube();
	model_mat_cpp = mvstack.pop();

	// --- Right Door Panel ---
	mvstack.push(model_mat_cpp);

	mat4 rightdoor = model_mat_cpp * translate(vec3(0.5f - 0.25f, -2.4f, 0.0f)) *  // Di chuyển tâm xoay ra cạnh trụ phải
		rotate_y(k) * translate(vec3(-doorWidth / 2.0f, doorHeight / 2.0f, 0.0f));

	model_mat_cpp = rightdoor * scale(vec3(doorWidth, doorHeight, doorDepth));
	setInt("color", 27); // Door Color
	cube();
	model_mat_cpp = mvstack.pop();
}


// vẽ tàu
void renderScene() {
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(-15.0f, 0.8f, 21.0f)) * scale(vec3(2.0f, 2.0f, 2.0f));//di chuyển toàn bộ mô hình
	generateTrack();
	drawTrackSupports();
	drawTrack();
	drawRollerCoaster();
	model_mat_cpp = mvstack.pop();
}

// vẽ xích đu
void renderBoat() {
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(-23.0f, 0.0f, -4.0f)) * scale(vec3(1.5f, 1.5f, 1.5f));//di chuyển toàn bộ mô hình
	drawBoatSwing();
	updateBoatSwing();
	model_mat_cpp = mvstack.pop();
}
// Vẽ sân khấu 
void renderStage() {
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(12.0f, 0.0f, 1.0f)) * rotate_y(90);//di chuyển toàn bộ mô hình
	sankhau();
	model_mat_cpp = mvstack.pop();
}
// vẽ vờ
void renderFlag() {
	
	// vẽ 3 cờ
	for (int i = 0; i < 24; i += 4) {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(i + -24.0f, 0.0f, -20.0f));//di chuyển toàn bộ mô hình
		drawFlag();
		model_mat_cpp = mvstack.pop();
	}

	for (int i = 0; i < 24; i += 4) {
		mvstack.push(model_mat_cpp);
		model_mat_cpp = model_mat_cpp * translate(vec3(i + 11.0f, 0.0f, -20.0f));//di chuyển toàn bộ mô hình
		drawFlag();
		model_mat_cpp = mvstack.pop();
	}
}
// vẽ cổng
void renderGate() {
	mvstack.push(model_mat_cpp);
	model_mat_cpp = model_mat_cpp * translate(vec3(22.5f, 1.9f, -22.0f)) * scale(vec3(2.0f, 2.0f, 2.0f));//di chuyển toàn bộ mô hình
	drawGate();
	model_mat_cpp = mvstack.pop();
}

void DisplayFunc(void)
{
	model_mat_cpp = identity_mat4();
	model_mat_location = glGetUniformLocation(ProgramId, "model_mat_shader");
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_mat_cpp.m);

	if (is_translated) {
		model_mat_cpp = model_mat_cpp * translate(vec3(T[0], T[1], T[2]));
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_mat_cpp.m);
	}
	if (is_rotated) {
		model_mat_cpp = model_mat_cpp * rotate_x(R[0]) * rotate_y(R[1]) * rotate_z(R[2]);
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_mat_cpp.m);
	}
	if (is_scaled) {
		model_mat_cpp = model_mat_cpp * scale(vec3(S[0], S[1], S[2]));  // Áp dụng tỉ lệ
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_mat_cpp.m);
	}

	glMatrixMode(GL_MODELVIEW);
	// Các tham số của hàm lookat
	vec3 eye(cameraX, cameraY, -cameraZ),
		at(cameraX + sin(beta), cameraY - pitch, cameraZ - cos(beta)),
		up(0.0f, 1.0f, 0.0f);

	view_mat_cpp = lookat(eye, at, up);
	view_mat_location = glGetUniformLocation(ProgramId, "view_mat_shader");
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat_cpp.m);

	// Phép chiếu phối cảnh
	float znear = 1.0f,
		zfar = 100.0f,
		fov = 90.0f,
		aspect = (float)CurrentWidth / CurrentHeight;
	projection_mat_cpp = perspective(fov, aspect, znear, zfar);
	projection_mat_location = glGetUniformLocation(ProgramId, "projection_mat_shader");
	glUniformMatrix4fv(projection_mat_location, 1, GL_FALSE, projection_mat_cpp.m);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	if (daynight) {
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f); // Màu tối
	}
	else {
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // Màu sáng
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniform1i(glGetUniformLocation(ProgramId, "use_texture"), GL_FALSE); // Không dán texture
	// Mặt đất
	ground();

	mvstack.push(model_mat_cpp); //PUSH_0

	//Toilet
	toilet::vetuong();
	toilet::updateWaterAngle(0.008); //Chinh toc do quay cua nuoc ở nhà VS
	toilet::vetoilet();

	//Quay kem
	int startTime = glutGet(GLUT_ELAPSED_TIME);  // Lấy thời gian trôi qua tính bằng mili giây
	float time = static_cast<float>(startTime) / 1000.0f;  // Chuyển sang giây
	quaykem::quay_kem(time);

	//Dong ho
	dongho::bigbag();

	voinuoc::updateWater(); 

	//Vòi nước
	voinuoc::hoanchinh();

	//Đu quay
	duquay::veDuQuay(3.5, 9, 10, 3, 3, 3);
	model_mat_cpp = mvstack.pop(); //POP_0

	//du quay thang
	temp = model_mat_cpp;

	//------------------------------------------di chuyen xich du o day-------------------------------
	vexichduthang(vec3(-15, 6.5, 2));
	model_mat_cpp = temp;

	//------------------------------------------di chuyen ban ghe o day-------------------------------
	table_full(vec3(-4, -0.5, -6));
	model_mat_cpp = temp;
	table_full(vec3(0, -0.5, -6));
	model_mat_cpp = temp;
	table_full(vec3(4, -0.5, -6));
	model_mat_cpp = temp;

	table_full(vec3(-4, -0.5, -8));
	model_mat_cpp = temp;
	table_full(vec3(0, -0.5, -8));
	model_mat_cpp = temp;
	table_full(vec3(4, -0.5, -8));
	model_mat_cpp = temp;

	table_full(vec3(-4, -0.5, -10));
	model_mat_cpp = temp;
	table_full(vec3(0, -0.5, -10));
	model_mat_cpp = temp;
	table_full(vec3(4, -0.5, -10));
	model_mat_cpp = temp;
	//------------------------------------------di chuyen chong chong o day-------------------------------
	chongchong_full(vec3(-1.8, -0.85, -2.5));
	model_mat_cpp = temp;
	chongchong_full(vec3(1.8, -0.85, -2.5));
	model_mat_cpp = temp;
	//////////////////////////////////////////////////////

	//Thùng giác
	veThungrac(1.85, -0.755, -1.8, 0.5, 0.7, 0.5);

	//Vẽ cây
	cay::vecay(20, 0, 13, 1.4, 1.6, 1);
	cay::vecay(20, 0, 8, 1.4, 1.6, 1);
	cay::vecay(20, 0, 3, 1.4, 1.6, 1);
	cay::vecay(20, 0, -2, 1.4, 1.6, 1);
	cay::vecay(20, 0, -7, 1.4, 1.6, 1);
	cay::vecay(20, 0, -12, 1.4, 1.6, 1);
	cay::vecay(20, 0, -17, 1.4, 1.6, 1);

	cay::vecay(25, 0, 13, 1.4, 1.6, 1);
	cay::vecay(25, 0, 8, 1.4, 1.6, 1);
	cay::vecay(25, 0, 3, 1.4, 1.6, 1);
	cay::vecay(25, 0, -2, 1.4, 1.6, 1);
	cay::vecay(25, 0, -7, 1.4, 1.6, 1);
	cay::vecay(25, 0, -12, 1.4, 1.6, 1);
	cay::vecay(25, 0, -17, 1.4, 1.6, 1);

	//Đèn đường
	denduong(17, 1.5, 11, 0.25, 5, 0.25, 0);
	denduong(13, 1.5, -11, 0.25, 5, 0.25, 0);
	denduong(13, 1.5, -5, 0.25, 5, 0.25, 0);
	hangrao::veHangRao(6, -0.5, -22, 1, 23, 0.1, 0.8, 0.1, 3, 0.1, 0.1, 0); //float startX, float startY, float startZ, float spacing( khoảng cách giữa các cột)
	//, int numColumns,float columnWidth, float columnHeight, float columnDepth,
	//float barWidth, float barHeight, float barDepth, float rotate) {
	hangrao::veHangRao(-28, -0.5, -22, 1, 23, 0.1, 0.8, 0.1, 3, 0.1, 0.1, 0);
	hangrao::veHangRao(-29, -0.5, 15, 1, 37, 0.1, 0.8, 0.1, 3, 0.1, 0.1, 90);
	hangrao::veHangRao(29, -0.5, 15, 1, 37, 0.1, 0.8, 0.1, 3, 0.1, 0.1, 90);
	hangrao::veHangRao(-28, -0.5, 16, 1, 57, 0.1, 0.8, 0.1, 3, 0.1, 0.1, 0);

	//tau
	renderScene();
	//xích du
	renderBoat();
	//san khau
	renderStage();
	// vẽ cờ
	renderFlag();
	// vẽ cổng
	renderGate();
	// Bầu trời
	if (daynight) {

	}
	else {
		GLuint texture;
		LoadTexture("blue1.png", &texture);
		glUniform1i(glGetUniformLocation(ProgramId, "use_texture"), GL_TRUE); // Dán texture
		glBindTexture(GL_TEXTURE_2D, texture); // Bind texture

		sky();
	}
	glutSwapBuffers();
}

//--------------Các hàm cho camera--------------
void updateCamera() {

	if (isJumping) {
		cameraY += jumpSpeed;
		jumpSpeed -= gravity;
		if (cameraY <= 0.0f) {
			cameraY = 0.0f;
			isJumping = false;
			isGrounded = true;
			jumpSpeed = 0.2f;
		}
	}
	glutPostRedisplay();
}
// ------------------------------------------
void ReshapeFunc(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	midWindowX = Width / 2;
	midWindowY = Height / 2;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}
// ------------------------------------------
bool phim1 = true, phim2 = true;
bool bool1 = true;
float step = 0.00005f;
void IdleFunc(void)
{
	if (bool1)
	{
		if (z >= 360 || z <= -360)
		{
			z = 0;
		}
		z += 0.02;

		t += step;
		if (t >= 2.0f || t <= -2.0f) {
			step = -step;
		}
	}

	if (quay >= 360)
	{
		quay = 0;
	}
	else
	{
		quay += 0.5;
	}

	onl1();
	glutPostRedisplay();
}
//--------Con lăn chuột---------------
void mouseWheel(int button, int direction, int x, int y) {
	if (direction > 0) {  // Cuộn chuột lên (phóng to)
		S[0] *= 1.1f;  // Phóng to đối tượng
		S[1] *= 1.1f;
		S[2] *= 1.1f;
	}
	else {  // Cuộn chuột xuống (thu nhỏ)
		S[0] /= 1.1f;  // Thu nhỏ đối tượng
		S[1] /= 1.1f;
		S[2] /= 1.1f;
	}

	// Sau khi thay đổi tỉ lệ, yêu cầu vẽ lại
	glutPostRedisplay();
}

// ------------------------------------------
void KeyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		glutLeaveMainLoop();
		break;
		//----------------camera----------------
	case 'w': //Camera gần
		cameraX += sin(beta) * moveSpeed;
		cameraZ -= cos(beta) * moveSpeed;
		break;
	case 's': //Camera xa
		cameraX -= sin(beta) * moveSpeed;
		cameraZ += cos(beta) * moveSpeed;
		break;
	case 'a': //Sang trái
		cameraX += sin(beta - 3.1415926535 / 2) * moveSpeed;
		cameraZ -= cos(beta - 3.1415926535 / 2) * moveSpeed;
		break;
	case 'd': //Sang phải
		cameraX += sin(beta + 3.1415926535 / 2) * moveSpeed;
		cameraZ -= cos(beta + 3.1415926535 / 2) * moveSpeed;
		break;
	case 'q': // Quay camera ra sau và nhìn ngược lại
		beta += 3.1415926535; // Quay 180 độ
		if (beta > 2 * 3.1415926535)
			beta -= 2 * 3.1415926535; // Giữ giá trị beta trong khoảng [0, 2π]
		cameraZ = -cameraZ; // Vị trí mắt không đổi
		break;
	case 'x':
		R[0] -= 10.0f;
		is_rotated = true;
		break;
	case 'X':
		R[0] += 10.0f;
		is_rotated = true;
		break;

	case 'y':
		R[1] -= 10.0f;
		is_rotated = true;
		break;
	case 'Y':
		R[1] += 10.0f;
		is_rotated = true;
		break;

	case 'z':
		R[2] -= 10.0f;
		is_rotated = true;
		break;
	case 'Z':
		R[2] += 10.0f;
		is_rotated = true;
		break;

	case '+': //Phóng to
		S[0] *= 1.05f;
		S[1] *= 1.05f;
		S[2] *= 1.05f;
		is_scaled = true;
		break;

	case '-': //Thu nhỏ
		S[0] /= 1.05f;
		S[1] /= 1.05f;
		S[2] /= 1.05f;
		is_scaled = true;
		break;

	case 'r': //Tịnh tiến sang phải
		T[0] -= 0.15f;
		is_translated = true;
		break;
	case 'l': //Tịnh tiến sang trái
		T[0] += 0.15f;
		is_translated = true;
		break;
	case ',':  //Tịnh tiến Xuống dưới
		T[1] += 1.0f;
		is_translated = true;
		break;
	case '.': //Tịnh tiến lên trên
		T[1] -= 1.0f;
		is_translated = true;
		break;
	case 'g':
		T[2] -= 0.15f;
		is_translated = true;
		break;
	case 'h':
		T[2] += 0.15f;
		is_translated = true;
		break;
	case '/': // Mở cửa nhà vs
		if (door_angle > -180.0f) // Giới hạn mở tối đa 180 độ
			door_angle -= 2.0f;
		if (toilet_angle > -180.0f)
			toilet_angle -= 2.0f;
		break;
	case '?': // Đóng cửa
		if (door_angle < 0.0f) // Giới hạn đóng hoàn toàn
			door_angle += 2.0f;
		if (toilet_angle < -90)
			toilet_angle += 2.0f;
		break;

	case 'f': //Mở cửa nhà ăn
		if (alpha3 > 90)
			alpha3 -= 5.0f;
		break;
	case 'F': //Đóng cửa nhà ăn
		if (alpha3 < 180)
			alpha3 += 5.0f;
		break;
	case '2':
		alpha_khoanuoc += 5.0f;
		dropWater = true;    // Kích hoạt trạng thái rơi
		waterPosY = 0.1f;    // Đặt lại vị trí ban đầu của giọt nước
		break;
	case '3':
		is_rotating = !is_rotating; // Bật/tắt trạng thái xả nước
		break;
	case '4': //Bật/tắt trạng thái quay đu quay
		isRotating1 = !isRotating1;
		break;
	case '5':  //Đóng/ mở thùng giác
		mo_thungrac = !mo_thungrac;
		break;
	case '6':  // Bật tắt đèn 
		moden = !moden;
		break;
	case '7':  // Trời sáng tối
		daynight = !daynight;
		glUniform1i(glGetUniformLocation(ProgramId, "daynight"), daynight);
		break;

		//----------------xich du thang----------------------------------

	case 'p': // p để xoay thủ công
		if (z >= 360 || z <= -360)
		{
			z = 0;
		}
		z += 5;
		break;
	case 'P':
		if (z >= 360 || z <= -360)
		{
			z = 0;
		}
		z -= 5; break;
		// t để tịnh tiến lên xuống thủ công
	case 't':
		if (t <= 1.8f && t >= -1.8f)
		{
			t += 0.05;
		}

		break;
	case 'T':
		if (t >= -1.5 && t <= 3.0f)
		{
			t -= 0.05;
		}
		break;
		// :/: để mở cửa đóng cửa
	case ':':
		if (c >= 0.4)
		{
			c += 0;
		}
		else
		{
			c += 0.025;
		}
		break;
	case ';':
		if (c <= 0 + 0.025)
		{
			c += 0;
		}
		else
		{
			c -= 0.025;
		}
		break;
	case 'C': // Open door
		if (k <= 90)
			k += 5.0f;
		break;
	case 'c': // Close door
		if (k >= -120)
			k -= 5.0f;
		break;
	case 'b':
		//bool1 để bật tắt chức năng tự động xoay
		bool1 = !bool1; break;
	case '8':
		trackSpeed += 0.01f; //Tăng tốc tàu
		break;
	case '9':
		trackSpeed -= 0.005f; //Giảm tốc
		break;
	}
}
// ----------------------------------------

void SpecialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		T[0] -= 0.05f;
		is_translated = true;
		break;
	case GLUT_KEY_RIGHT:
		T[0] += 0.05f;
		is_translated = true;
		break;
	case GLUT_KEY_UP:
		T[1] += 0.05f;
		is_translated = true;
		break;
	case GLUT_KEY_DOWN:
		T[1] -= 0.05f;
		is_translated = true;
		break;
	case GLUT_KEY_ALT_R:
		T[2] -= 0.05f;
		is_translated = true;
		break;
	case GLUT_KEY_CTRL_R:
		T[2] += 0.05f;
		is_translated = true;
		break;
	}
}
void danhsachlenh() {
	cout << "g/h : Tinh tien vat the ve gan/xa camera" << endl;
	cout << "w/s : Dieu khien camera tien/lui  " << endl;
	cout << "a/d : Dieu khien camera sang trai/phai" << endl;;
	cout << "r/l : Dieu khien vat the sang trai/phai" << endl;
	cout << "q   : Nhin nguoc lai 180 do" << endl;
	cout << "+/- : Phong to/Thu nho (+ hoac cuon chuot) " << endl;
	cout << "./, : Dua camera len tren/xuong duoi" << endl;
	cout << "//? : Mo/Dong cua nha VS + Nap Toilet mo ra" << endl;
	cout << "x/X : Quay quanh truc Ox" << endl;
	cout << "y/Y : Quay quanh truc Oy" << endl;
	cout << "z/Z : Quay quanh truc Oz" << endl;
	cout << "f/F : Dong/Mo cua nha an" << endl;
	cout << "t/T : Tinh tien thu cong xich du thang" << endl;
	cout << "c/C : Dong mo cua chinh " << endl;
	cout << "2   : Bat/tat trang thai mo voi nuoc rua tay" << endl;
	cout << "3   : Bat/tat trang thai xa nuoc nha VS" << endl;
	cout << "4   : Quay/Dung vong quay" << endl;
	cout << "5   : Dong/mo thung giac" << endl;
	cout << "6   : Bat/Tat den duong" << endl;
	cout << "7   : Che do ngay/dem" << endl;
	cout << "8   : Tang toc tau," << endl;
	cout << "9   : Giam toc tau" << endl;
	cout << "b   : Bat/tat xich du thang" << endl;
	cout << ":   : Dong/mo cua xich du thang" << endl;

	cout << endl;
}
// ------------------------------------------
int main(int argc, char* argv[])
{
	danhsachlenh();
	glutInit(&argc, argv);
	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize(CurrentWidth, CurrentHeight);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Computer Graphic: City Park");

	glewExperimental = GL_TRUE;
	glewInit();

	CreatVaoVbo();
	CreatShaders();

	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
	glutIdleFunc(IdleFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutCloseFunc(CloseFunc);
	glutMouseWheelFunc(mouseWheel);
	glutMainLoop();
	return 0;
}
