#version 330 core
#extension GL_ARB_gpu_shader_fp64 : require

vec4 hsva(vec4 c){
    vec4 K = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(c.x + K.xyz) * 6.0 - K.www);
    vec3 rgb = c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
    return vec4(rgb, c.w);
}

out vec4 fragColor;

uniform double scale_x;
uniform double scale_y;
uniform double offset_x;
uniform double offset_y;
uniform double cursorWidth;
uniform double cursorHeight;
uniform double indicatorSize;
uniform int steps;
uniform float invStep;
uniform float angle;
uniform float radius;
uniform dvec2 constPoint;
uniform int power;
uniform bool colour;
uniform dvec2 mousePos;

const double ESCAPE_SQ = 4.0;

dvec2 cmult(dvec2 x, dvec2 y){
	return dvec2(x.x*y.x-x.y*y.y,x.x*y.y+x.y*y.x);
}

dvec2 cpow2(dvec2 x) {
    return dvec2(x.x*x.x - x.y*x.y, 2.0*x.x*x.y);
}
dvec2 cpow3(dvec2 x) {
    dvec2 sq = cpow2(x);
    return dvec2(sq.x*x.x - sq.y*x.y, sq.x*x.y + sq.y*x.x);
}
dvec2 cpow(dvec2 x,int p){
	switch(p){
		case 0:
			return dvec2(1.0,0.0);
			break;
		case 1:
			return x;
			break;
		case 2:
			return cpow2(x);
			break;
		case 3:
			return cpow3(x);
			break;
		default:
			dvec2 result = dvec2(1.0,0.0);
			dvec2 base = x;
			int expp = p;
			while(expp>0){
				if((expp & 1) == 1) result = cmult(result,base);
				base = cpow2(base);
				expp >>=1;
			}
			return result;
	}
}

double norm(dvec2 x){
	return dot(x,x);
}

dvec2 f(dvec2 z,dvec2 c, int power){
	return cpow(z,power)+c;
}
dvec2 g(dvec2 z,dvec2 c){
	return cpow(z,3)+c;
}


void main(){
    dvec2 point = dvec2(gl_FragCoord.x * scale_x + offset_x, gl_FragCoord.y * scale_y + offset_y); 


	if(norm(point-constPoint)<indicatorSize){
		fragColor = vec4(1.0,0.0,0.0,1.0);
	}else if( (abs(point.x-mousePos.x) < cursorWidth && abs(point.y-mousePos.y) < cursorHeight) || (abs(point.y-mousePos.y) < cursorWidth && abs(point.x-mousePos.x) < cursorHeight)){
		fragColor = vec4(0.0,1.0,0.0,1.0);
	}else if(power==1){
		dvec2 c = point;
		point = dvec2(0.0,0.0);
		dvec2 point2  = dvec2(0.0,0.0);

		bool escape = false;
		int numStep = 0;

		for(int i=0;i<steps;i++){
			point = cpow2(point) + c;
			point2 = cpow3(point2) + c;
			if(norm(point)>ESCAPE_SQ || norm(point2) > ESCAPE_SQ){
				escape = true;
				numStep = i;
				break;
			}
		}
		if(escape){
			float normStep = float(numStep) * invStep;
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
			if(norm(point)>ESCAPE_SQ){
				escape = true;
				numStep = i;
				break;
			}
		}
		if(escape){
			float normStep = float(numStep) * invStep;
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
