#version 440 core

in vec3 fragPosition;
in vec3 fragNormal;
in float fragAlong; // Surface coordinate along the stem.
in float fragAround; // Surface coordinate around the stem.
uniform vec3 eyePosition;

out vec4 outColor;

float cap(const in float x) 
{

    return -abs(fma(x, 2.0f, -1.0f)) + 1.0f;
}

void main(void) {
    const vec4 ambient = vec4(0.05f, 0.1f, 0.05f, 1.0f);
    const vec4 diffuseColor = vec4(0.2f, 0.5f, 0.2f, 1.0f);
    const vec4 specularColor = vec4(0.6f, 0.6f, 0.6f, 1.0f);

    // Стандартное освещение по Фонгу
    const vec3 lightPosition = vec3(0.0f, 0.5f, 0.0f);

    vec3 L = normalize(lightPosition - fragPosition);
    vec3 E = normalize(eyePosition - fragPosition);
    vec3 R = normalize(reflect(-L, fragNormal));

    float diffuseCoeff = max(dot(fragNormal, L), 0.0f);
    float specularCoeff = pow(max(dot(R, E), 0.0f), 32.0f);

    vec4 diffuse = clamp(diffuseColor * diffuseCoeff, 0.0f, 1.0f) * (fragAlong * 0.5f + 0.5f);
    vec4 specular = clamp(specularColor * specularCoeff, 0.0f, 1.0f);

    outColor = ambient + diffuse + specular;
    outColor.a = 1.0;
}
