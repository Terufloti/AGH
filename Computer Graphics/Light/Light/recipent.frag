#version 330 core
out vec4 fragmentColor;
in vec3 vertexNormal;
in vec3 fragmentPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPosition;
uniform float ambientLight;
uniform float diffuseLight;
uniform float specularLight;
void main(){
   float ambientStrenght = 0.15;
   float specStrenght = 0.4;
   vec3 ambientColor = ambientStrenght * lightColor * ambientLight;
   vec3 normalNormalized = normalize(vertexNormal);
   vec3 lightDirection = normalize(lightPosition - fragmentPosition);
   float diffuse = max(dot(normalNormalized, lightDirection), 0.0);
   vec3 diffuseColor = diffuse * lightColor * diffuseLight;
   vec3 viewDirection = normalize(viewPosition - fragmentPosition);
   vec3 reflectionDirection = reflect(-lightDirection, normalNormalized);
   float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);
   vec3 specularColor = spec * lightColor * specStrenght * specularLight;
   fragmentColor = vec4(objectColor * (ambientColor + diffuseColor + specularColor), 1.0);
}