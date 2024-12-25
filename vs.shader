//vs.shader
#version 460

layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in vec4 vertex_normal;
layout(location = 3) in vec2 texture_position; //Thêm

uniform mat4 projection_mat_shader;
uniform mat4 view_mat_shader;
uniform mat4 model_mat_shader;

uniform int color;

out vec3 position_eye;
out vec3 normal_eye;
out vec4 fragment_color;
out vec2 texture_coordinates; //Thêm

void main(void)
{
	texture_coordinates = texture_position; //Thêm
	fragment_color = vertex_color; //Thêm

	position_eye = vec3(view_mat_shader * model_mat_shader * vertex_position);
	normal_eye = vec3(view_mat_shader * model_mat_shader * vertex_normal);
	
	if (color == 1)    fragment_color = vec4(0.48, 0.21, 0.1, 1.0); //brown
	if (color == 2)    fragment_color = vec4(0.23, 0.23, 0.23, 1.0); //grey  
	if (color == 3)    fragment_color = vec4(0.75, 0.02, 0.02, 1.0); //red
	if (color == 4)    fragment_color = vec4(0.02, 0.75, 0.02, 1.0); //blue
	if (color == 5)    fragment_color = vec4(0, 1, 0, 1); //green
	if (color == 6)    fragment_color = vec4(0.196078443137, 0.50980392157, 0.96470588235, 0); //dark blue
	if (color == 7)    fragment_color = vec4(0.80, 0.25, 0.03, 1.0); //orange  
	if (color == 8)    fragment_color = vec4(0.02, 0.68, 0.65, 1.0); //greenagai
	if (color == 9)    fragment_color = vec4(0.7, 0.7, 0.7, 0);//glass
	if (color == 10)   fragment_color = vec4(0.75, 0.75, 0.75, 1.0);//sliver
	if (color == 11)   fragment_color = vec4(1.0, 1.0, 0.75, 1.0);//yellowlight
	if (color == 12)   fragment_color = vec4(0.83, 0.83, 0.83, 1.0);//lightgray
	if (color == 13)   fragment_color = vec4(0.803921568627451, 0.5215686274509804, 0.2470588235294118, 1.0);//peru
	if (color == 14)   fragment_color = vec4(0.95, 0.95, 0.90, 1.0);//yellow_kem
	if (color == 15)   fragment_color = vec4(0.94, 0.9, 0.55, 0.01);//khaki
	if (color == 16)   fragment_color = vec4(0.5294117647, 0.8078431373, 0.9215686275, 1.0); // skyblue
	if (color == 17)   fragment_color = vec4(0.545, 0.271, 0.075, 1.0); // saddle brown
	if (color == 18)   fragment_color = vec4(0.486, 0.988, 0.000, 1.0); // chartreuse
	if (color == 19)   fragment_color = vec4(0.678, 1.000, 0.184, 1.0); // lawn green
	if (color == 20)   fragment_color = vec4(0.000, 0.392, 0.000, 1.0); // dark green
	if (color == 21)   fragment_color = vec4(0.941, 0.902, 0.549, 1.0); // light khaki
	if (color == 22)   fragment_color = vec4(0.941, 0.502, 0.502, 1.0); // rosy brown
	if (color == 23)   fragment_color = vec4(0.627, 0.322, 0.176, 1.0); // sienna
	if (color == 24)   fragment_color = vec4(0.529, 0.808, 0.980, 1.0); // deep sky blue
	if (color == 25)   fragment_color = vec4(0.255, 0.412, 0.882, 1.0); // royal blue
	if (color == 26)   fragment_color = vec4(0.941, 0.973, 1.000, 1.0); // alice blue
	if (color == 27)   fragment_color = vec4(1.000, 0.647, 0.000, 1.0); // orange
	if (color == 28)   fragment_color = vec4(0.941, 0.902, 0.800, 1.0); // wheat
	if (color == 29)   fragment_color = vec4(0.824, 0.706, 0.549, 1.0); // burlywood
	if (color == 30)   fragment_color = vec4(0.251, 0.878, 0.816, 1.0); // medium turquoise
	if (color == 31)   fragment_color = vec4(1.000, 0.271, 0.000, 1.0); // orange red
	if (color == 32)   fragment_color = vec4(0.545, 0.000, 0.545, 1.0); // dark magenta
	if (color == 33)   fragment_color = vec4(1.000, 0.753, 0.796, 1.0); // pink
	if (color == 34)   fragment_color = vec4(0.196, 0.804, 0.196, 1.0); // lime green
	if (color == 35)   fragment_color = vec4(0.498, 1.000, 0.831, 1.0); // aquamarine
	if (color == 36)   fragment_color = vec4(1.000, 1.000, 0.000, 1.0); // yellow
	if (color == 37)   fragment_color = vec4(0.933, 0.910, 0.667, 1.0); // pale goldenrod
	if (color == 38)   fragment_color = vec4(0.000, 0.980, 0.604, 1.0); // spring green
	if (color == 39)   fragment_color = vec4(0.690, 0.769, 0.871, 1.0); // light steel blue
	if (color == 40) fragment_color = vec4(0.824, 0.824, 0.824, 1.0); // gainsboro
	if (color == 41) fragment_color = vec4(0.545, 0.545, 0.545, 1.0); // dimgray 
	if (color == 42) fragment_color = vec4(0.118, 0.565, 1.000, 1.0); // dodgerblue 
	if (color == 43) fragment_color = vec4(1.000, 0.843, 0.000, 1.0); // gold 
	if (color == 44) fragment_color = vec4(0.545, 0.271, 0.075, 1.0); // saddleBrown 
	if (color == 45) fragment_color = vec4(1.000, 0.412, 0.706, 1.0); // hotpink 
	if (color == 46) fragment_color = vec4(0.678, 1.000, 0.184, 1.0); // greenyellow 
	if (color == 47) fragment_color = vec4(0.196, 0.804, 0.196, 1.0); // limegreen 

	if (color == 98)   fragment_color = vec4(0.95, 0.95, 0.90, 1.0); //white
	if (color == 99)   fragment_color = vec4(0.03, 0.03, 0.03, 1.0); //black

	gl_Position = projection_mat_shader * vec4(position_eye, 1.0);
}