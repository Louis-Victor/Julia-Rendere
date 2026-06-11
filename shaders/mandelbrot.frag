#version 330 core
#extension GL_ARB_gpu_shader_fp64 : require

vec4 hsva(vec4 c){
    vec4 K = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(c.x + K.xyz) * 6.0 - K.www);
    vec3 rgb = c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
    return vec4(rgb, c.w);
}

in vec4 gl_FragCoord;
out vec4 fragColor;

uniform double width;
uniform double height;
uniform dvec2 shift;
uniform double zoom;
uniform int steps;
uniform float angle;
uniform float radius;
uniform dvec2 constPoint;
uniform int power;
uniform bool colour;

dvec2 cmult(dvec2 x, dvec2 y){
	return dvec2(x.x*y.x-x.y*y.y,x.x*y.y+x.y*y.x);
}
dvec2 cpow(dvec2 x,int p){
	dvec2 result = x;
	for(int i=1;i<p;i++){
		result = cmult(result,x);
	}
	return result;
}

double norm(dvec2 x){
	return x.x*x.x+x.y*x.y;
}

dvec2 f(dvec2 z,dvec2 c, int power){
	return cpow(z,power)+c;
}
dvec2 g(dvec2 z,dvec2 c){
	return cpow(z,3)+c;
}

void main(){
    dvec2 point = (2.0/zoom)*gl_FragCoord.xy/height - (1.0/zoom)*vec2(width/height,1.0)+shift;


	if(norm(point-constPoint)<0.00001/(zoom*zoom)){
		fragColor = vec4(1.0,0.0,0.0,1.0);
	}else{
		// JULIA
		//dvec2 c = constPoint;//radius*vec2(cos(angle),sin(angle));
		// MANDELBROT
		dvec2 c = point;
		point = dvec2(0.0,0.0);

		bool escape = false;
		int numStep = 0;

		for(int i=0;i<steps;i++){
			point = f(point,c,power);
			//point3 = g(point,c);
			if(norm(point)>4){
			//if(norm(point) > 4 || norm(point3) > 4){
				escape = true;
				numStep = i;
				break;
			}
		}
		if(escape){
			float normStep = float(numStep) / float(steps);
			//float timeMantisa = time - floor(time);
			//float aval = atan(point.y/point.x)/3.141592653589793f;
			if(colour){
				// COLOUR
				fragColor = hsva(vec4(normStep,0.6f,1.0f,1.0f));
			}else{
				// MONOCHROME
				fragColor = hsva(vec4(0.0,0.0,1.0-normStep,1.0f));
			}
		}else{
			fragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}

    //vec2 uv = gl_FragCoord.xy / vec2(width,height);
    //fragColor = vec4(uv, 0.0, 1.0);
    
}
