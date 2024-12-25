#version 460
#define MAX_LIGHTS 10
in vec3 position_eye, normal_eye;
uniform mat4 view_mat_shader;

in vec2 texture_coordinates;   //Thêm
uniform bool use_texture;  //Thêm
uniform sampler2D basic_texture;  //Thêm
in vec4 fragment_color;
out vec4 out_color;

// Các biến uniform để bật/tắt từng nguồn sáng
uniform bool enable_light_1; // Bật/tắt nguồn sáng 1
uniform bool enable_light_2; // Bật/tắt nguồn sáng 2
uniform int color;  // Nhận giá trị từ mã C++ cho color
// đèn
// Uniform để bật/tắt ánh sáng và màu riêng biệt của đèn
uniform bool enable_light_3; // Ánh sáng từ phần đèn chiếu xuống
uniform vec3 light_color_custom; // Màu riêng của đèn chiếu sáng
uniform vec4 light_position_custom[MAX_LIGHTS]; // Mảng chứa vị trí của các đèn
// Trời tối sáng
uniform bool daynight;
uniform int soden;
void main() {
    vec3 final_color = vec3(0);
    if(daynight){
        // Kiểm tra xem có sử dụng màu xanh bầu trời hay không
    if (color == 16) {
        final_color = vec3(0.5294117647, 0.8078431373, 0.9215686275); // Màu skyblue
    } 
    else {
        // Nếu không phải là màu skyblue, tiếp tục với việc tính toán ánh sáng
        //-----------Nguồn sáng 1-------------
        if (enable_light_1) {
            vec3 light_position_world_1 = vec3(0.0, 50, 0.0);
            vec3 light_position_eye_1 = vec3(view_mat_shader * vec4(light_position_world_1, 1.0));
            vec3 position_viewer = vec3(0.0, 0.0, 0.0);
            vec3 n_eye = normalize(normal_eye);

            vec3 distance_to_light_eye_1 = light_position_eye_1 - position_eye;
            vec3 direction_to_light_eye_1 = normalize(distance_to_light_eye_1);
            vec3 surface_to_viewer_eye_1 = normalize(position_viewer - position_eye);

            float dot_prod_1 = max(dot(direction_to_light_eye_1, n_eye), 0.0);
            //vec3 Ld_1 = vec3(0.7, 0.7, 0.7);
            vec3 Ld_1 = vec3(0.1, 0.1, 0.1);
            vec3 Kd_1 = vec3(1.0, 1.0, 1.0);
            vec3 Id_1 = Ld_1 * Kd_1 * dot_prod_1;

            vec3 half_way_eye_1 = normalize(surface_to_viewer_eye_1 + direction_to_light_eye_1);
            float dot_prod_specular_1 = max(dot(half_way_eye_1, n_eye), 0.0);
            float specular_factor_1 = pow(dot_prod_specular_1, 20.0);
            vec3 Ls_1 = vec3(0.2, 0.2, 0.2);
            vec3 Ks_1 = vec3(1.0, 1.0, 1.0);
            vec3 Is_1 = Ls_1 * Ks_1 * specular_factor_1;

            //vec3 La_1 = vec3(0.2, 0.2, 0.2);
            vec3 La_1 = vec3(0.05, 0.05, 0.05);
            vec3 Ka_1 = vec3(1.0, 1.0, 1.0);
            vec3 Ia_1 = La_1 * Ka_1;

            final_color += (Is_1 + Id_1 + Ia_1);
        }

        //-----------Nguồn sáng 2-------------
        if (enable_light_2) {
            vec3 light_position_world_2 = vec3(0, 30.0, -20.0);  //Chỉnh ở đây là các cánh cửa sẽ có màu nha
            vec3 light_position_eye_2 = vec3(view_mat_shader * vec4(light_position_world_2, 1.0));
            vec3 position_viewer = vec3(0.0, 0.0, 0.0);
            vec3 n_eye = normalize(normal_eye);

            vec3 distance_to_light_eye_2 = light_position_eye_2 - position_eye;
            vec3 direction_to_light_eye_2 = normalize(distance_to_light_eye_2);
            vec3 surface_to_viewer_eye_2 = normalize(position_viewer - position_eye);

            float dot_prod_2 = max(dot(direction_to_light_eye_2, n_eye), 0.0);
            //vec3 Ld_2 = vec3(0.7, 0.7, 0.7);
            vec3 Ld_2 = vec3(0.1, 0.1, 0.1);
            vec3 Kd_2 = vec3(1, 0.5, 0);
            vec3 Id_2 = Ld_2 * Kd_2 * dot_prod_2;

            vec3 half_way_eye_2 = normalize(surface_to_viewer_eye_2 + direction_to_light_eye_2);
            float dot_prod_specular_2 = max(dot(half_way_eye_2, n_eye), 0.0);
            float specular_factor_2 = pow(dot_prod_specular_2, 50.0);
            vec3 Ls_2 = vec3(0.8, 0.8, 1.0);
            vec3 Ks_2 = vec3(0.7, 0.7, 0.7);
            vec3 Is_2 = Ls_2 * Ks_2 * specular_factor_2;

            //vec3 La_2 = vec3(0.1, 0.1, 0.3);
            vec3 La_2 = vec3(0.05, 0.05, 0.5);
            vec3 Ka_2 = vec3(0.5, 0.5, 0.5);
            vec3 Ia_2 = La_2 * Ka_2;

            final_color += (Is_2 + Id_2 + Ia_2);
        }
    }
    if (enable_light_3) {
       for (int i = 0 ; i < 3; i++){
            vec3 light_position_eye_3 = vec3(view_mat_shader * light_position_custom[i]); // Vị trí đèn
            vec3 n_eye = normalize(normal_eye);
            vec3 direction_to_light_eye_3 = normalize(light_position_eye_3);
            // Tính khoảng cách từ nguồn sáng đến điểm chiếu sáng
            float distance_to_light_3 = length(light_position_eye_3 - position_eye);

            // Áp dụng attenuation (suy giảm ánh sáng theo khoảng cách)
            float attenuation = 1.0 / (1.0 + distance_to_light_3 * distance_to_light_3 * 0.05);
            // Hệ số tăng cường độ ánh sáng
            float scale_factor = 10.0;  

            float dot_prod_3 = max(dot(direction_to_light_eye_3, n_eye), 0.0);
            vec3 Id_3 = light_color_custom * dot_prod_3 * attenuation * scale_factor; // Màu chiếu sáng riêng
        
            vec3 surface_to_viewer_eye_3 = normalize(-position_eye);
            vec3 half_way_eye_3 = normalize(surface_to_viewer_eye_3 + direction_to_light_eye_3);
            float dot_prod_specular_3 = max(dot(half_way_eye_3, n_eye), 0.0);
            float specular_factor_3 = pow(dot_prod_specular_3, 30.0);
            vec3 Is_3 = light_color_custom * specular_factor_3 * attenuation * scale_factor;

            vec3 Ia_3 = light_color_custom * 0.1 * attenuation * scale_factor; // Ánh sáng môi trường nhẹ

            final_color += (Id_3 + Is_3 + Ia_3);
    }
                
  }

    
    final_color = final_color * fragment_color.rgb; // Kết hợp với màu nền từ fragment_color

    out_color = vec4(final_color, 1.0);
}

    else{
    // Kiểm tra xem có sử dụng màu xanh bầu trời hay không
    if (color == 16) {
        final_color = vec3(0.5294117647, 0.8078431373, 0.9215686275); // Màu skyblue
    } 
    else {
        // Nếu không phải là màu skyblue, tiếp tục với việc tính toán ánh sáng
        //-----------Nguồn sáng 1-------------
        if (enable_light_1) {
            vec3 light_position_world_1 = vec3(0.0, 50, 0.0);
            vec3 light_position_eye_1 = vec3(view_mat_shader * vec4(light_position_world_1, 1.0));
            vec3 position_viewer = vec3(0.0, 0.0, 0.0);
            vec3 n_eye = normalize(normal_eye);

            vec3 distance_to_light_eye_1 = light_position_eye_1 - position_eye;
            vec3 direction_to_light_eye_1 = normalize(distance_to_light_eye_1);
            vec3 surface_to_viewer_eye_1 = normalize(position_viewer - position_eye);

            float dot_prod_1 = max(dot(direction_to_light_eye_1, n_eye), 0.0);
            vec3 Ld_1 = vec3(0.7, 0.7, 0.7);
            vec3 Kd_1 = vec3(1.0, 1.0, 1.0);
            vec3 Id_1 = Ld_1 * Kd_1 * dot_prod_1;

            vec3 half_way_eye_1 = normalize(surface_to_viewer_eye_1 + direction_to_light_eye_1);
            float dot_prod_specular_1 = max(dot(half_way_eye_1, n_eye), 0.0);
            float specular_factor_1 = pow(dot_prod_specular_1, 20.0);
            vec3 Ls_1 = vec3(0.5, 0.5, 0.5);
            vec3 Ks_1 = vec3(1.0, 1.0, 1.0);
            vec3 Is_1 = Ls_1 * Ks_1 * specular_factor_1;

            vec3 La_1 = vec3(0.2, 0.2, 0.2);
            vec3 Ka_1 = vec3(1.0, 1.0, 1.0);
            vec3 Ia_1 = La_1 * Ka_1;

            final_color += (Is_1 + Id_1 + Ia_1);
        }

        //-----------Nguồn sáng 2-------------
        if (enable_light_2) {
            vec3 light_position_world_2 = vec3(0, 30.0, -20.0);  //Chỉnh ở đây là các cánh cửa sẽ có màu nha
            vec3 light_position_eye_2 = vec3(view_mat_shader * vec4(light_position_world_2, 1.0));
            vec3 position_viewer = vec3(0.0, 0.0, 0.0);
            vec3 n_eye = normalize(normal_eye);

            vec3 distance_to_light_eye_2 = light_position_eye_2 - position_eye;
            vec3 direction_to_light_eye_2 = normalize(distance_to_light_eye_2);
            vec3 surface_to_viewer_eye_2 = normalize(position_viewer - position_eye);

            float dot_prod_2 = max(dot(direction_to_light_eye_2, n_eye), 0.0);
            vec3 Ld_2 = vec3(0.7, 0.7, 0.7);
            vec3 Kd_2 = vec3(1, 0.5, 0);
            vec3 Id_2 = Ld_2 * Kd_2 * dot_prod_2;

            vec3 half_way_eye_2 = normalize(surface_to_viewer_eye_2 + direction_to_light_eye_2);
            float dot_prod_specular_2 = max(dot(half_way_eye_2, n_eye), 0.0);
            float specular_factor_2 = pow(dot_prod_specular_2, 50.0);
            vec3 Ls_2 = vec3(0.8, 0.8, 1.0);
            vec3 Ks_2 = vec3(0.7, 0.7, 0.7);
            vec3 Is_2 = Ls_2 * Ks_2 * specular_factor_2;

            vec3 La_2 = vec3(0.1, 0.1, 0.3);
            vec3 Ka_2 = vec3(0.5, 0.5, 0.5);
            vec3 Ia_2 = La_2 * Ka_2;

            final_color += (Is_2 + Id_2 + Ia_2);
        }
    }
    if (enable_light_3) {
       for (int i = 0 ; i < 3; i++){
            vec3 light_position_eye_3 = vec3(view_mat_shader * light_position_custom[i]); // Vị trí đèn
            vec3 n_eye = normalize(normal_eye);
            vec3 direction_to_light_eye_3 = normalize(light_position_eye_3);
            // Tính khoảng cách từ nguồn sáng đến điểm chiếu sáng
            float distance_to_light_3 = length(light_position_eye_3 - position_eye);

            // Áp dụng attenuation (suy giảm ánh sáng theo khoảng cách)
            float attenuation = 1.0 / (1.0 + distance_to_light_3 * distance_to_light_3 * 0.05);
            // Hệ số tăng cường độ ánh sáng
            float scale_factor = 5.0;  

            float dot_prod_3 = max(dot(direction_to_light_eye_3, n_eye), 0.0);
            vec3 Id_3 = light_color_custom * dot_prod_3 * attenuation * scale_factor; // Màu chiếu sáng riêng
        
            vec3 surface_to_viewer_eye_3 = normalize(-position_eye);
            vec3 half_way_eye_3 = normalize(surface_to_viewer_eye_3 + direction_to_light_eye_3);
            float dot_prod_specular_3 = max(dot(half_way_eye_3, n_eye), 0.0);
            float specular_factor_3 = pow(dot_prod_specular_3, 30.0);
            vec3 Is_3 = light_color_custom * specular_factor_3 * attenuation * scale_factor;

            vec3 Ia_3 = light_color_custom * 0.1 * attenuation * scale_factor; // Ánh sáng môi trường nhẹ

            final_color += (Id_3 + Is_3 + Ia_3);
    }
                
  }

    if (use_texture) {
        vec4 texel = texture(basic_texture, texture_coordinates);
        final_color = texel.rgb * final_color;  // Cách này giữ lại ánh sáng và chỉ áp dụng texture
    }
    final_color = final_color * fragment_color.rgb*1.5; // Kết hợp với màu nền từ fragment_color

    out_color = vec4(final_color, 1.0);
}}
    
