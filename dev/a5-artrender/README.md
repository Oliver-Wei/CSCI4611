CSci 4611 Assignment 5 README
Yingxin Wei

1. For phong shading:

Almost all the job needed to be done is in phone.frag, which is similar to gouraud.vert.
The key parameters in the phong equation are listed below:
    v is given as position_in_eye_space
    l is the unit vector of light_in_eye_space
    e is the negative of unit vector of v
    n should be the unit vector of normal_in_eye_space
    h is the halfway vector

According to equation, the ambient, diffuse, and specular could be calculated as:
    vec4 ambient = ka * Ia;
    vec4 diffuse = kd * Id * max(dot(n, l), 0);
    vec4 specular = ks * Is * pow(max(dot(n, h), 0), s);

The color should be the sum of these three parameters.
    color = ambient + diffuse + specular;

2. For art shading:

I use the same way to calculate the basic parameters such as v, l, e, n, and h, while adding the texture to the diffuse and specular and keep ambient the same. Textures are added by texture().

        float diff_intensity = (dot(n,l) + 1) / 2;
        vec2 diffuse_texture = vec2(diff_intensity, 1.0 - diff_intensity);
        vec4 diffuse = kd * Id * texture(diffuse_ramp, diffuse_texture);

        float specular_intensity = pow(max(dot(n, h), 0), s);
        vec2 specular_texture = vec2(specular_intensity, 1.0 - specular_intensity);
        vec4 specular = texture(specular_ramp, specular_texture);

For outline, firstly the right and left normal are transfered into eye space by multiplying with normal matrix, then vertex shader would determine if the edge is the outline by examing whether the equation is smaller than zero.

    vec3 e = (model_view_matrix * vec4(vertex,1)).xyz;
    vec3 final_vertex = vertex;
    vec3 nl = (normal_matrix * vec4(left_normal,1)).xyz;
    vec3 nr = (normal_matrix * vec4(right_normal,1)).xyz;

    if (dot(e, nl) * dot(e, nr) < 0.0)

If true, then the final vertex would be changed as: `final_vertex = vertex + thickness * normal` to draw black line on the edge of the model.
