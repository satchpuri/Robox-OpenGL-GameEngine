#version 430
in vec2 fraguv;
uniform sampler2D myTexture;

void main() 
{	
	gl_FragColor = texture(myTexture, fraguv);
}