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
uniform int steps;
uniform float invStep;
uniform float angle;
uniform float radius;
uniform dvec2 constPoint;
uniform int power;
uniform bool colour;

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

dvec2 f(dvec2 z,dvec2 c, int _power){
	return cpow(z,_power)+c;
}
dvec2 g(dvec2 z,dvec2 c){
	return cpow(z,3)+c;
}

void main(){
	dvec2 point = dvec2(gl_FragCoord.x * scale_x + offset_x, gl_FragCoord.y * scale_y + offset_y);

	// JULIA
    dvec2 c = constPoint;//radius*vec2(cos(angle),sin(angle));
	// MANDELBROT
	//dvec2 c = point;
	//point = dvec2(0.0,0.0);

    bool escape = false;
    int numStep = 0;


	if(power == 1){
		dvec2 point2 = point;
		for(int i=0;i<steps;i++){
			point  = cpow2(point) + c; 
			point2 = cpow3(point2) + c; 
			if(norm(point)>ESCAPE_SQ || norm(point2)>ESCAPE_SQ){
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
				fragColor = hsva(vec4(normStep,0.6f,1.0f,1.0f));
			}else{
				fragColor = hsva(vec4(0.0,0.0,1.0-normStep,1.0f));
			}
		}else{
			fragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}else{
		for(int i=0;i<steps;i++){
			point = f(point,c,power);
			//point3 = g(point,c);
			if(norm(point)>ESCAPE_SQ){
			//if(norm(point) > 4 || norm(point3) > 4){
				escape = true;
				numStep = i;
				break;
			}
		}
		if(escape){
			float normStep = float(numStep)*invStep;
			//float timeMantisa = time - floor(time);
			//float aval = atan(point.y/point.x)/3.141592653589793f;
			if(colour){
				fragColor = hsva(vec4(normStep,0.6f,1.0f,1.0f));
			}else{
				fragColor = hsva(vec4(0.0,0.0,1.0-normStep,1.0f));
			}
		}else{
			fragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	//fragColor = vec4(1.0,0.0,0.0,1.0);
    //vec2 uv = gl_FragCoord.xy / vec2(width,height);
    //fragColor = vec4(uv, 0.0, 1.0);
    
}
