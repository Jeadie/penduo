#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include<math.h> 

int ITERATIONS = 100;
float STEP_SIZE = 0.01; 

float MASS = 10.0; 
float LENGTH = 1.2; 
float GRAVITY = 9.8; 

typedef float (*differential_fn)(float*);

int main(int argc, char *argv[])
{

  // Consider vector (p_1, p_2, theta_1, theta_2) at time t, 
  // vector is stored at data[4*t]
  // For individual value with index j < 4 data[4*t + j]
  float* data= (float *) malloc(4* ITERATIONS * sizeof(float)); 
  
  // Initial Conditions
  data[0] = 0.0;
  data[1] = 0.0;
  data[2] = 0.0;
  data[3] = 0.0;

  // Step SI

  // Derivatives 
  differential_fn* differentials = malloc(4*sizeof(differential_fn));
  differentials[0] = d_p_1; 
  differentials[1] = d_p_2; 
  differentials[2] = d_theta_1; 
  differentials[3] = d_theta_2;

  // Apply Runga Kutta 
  for (int i =0; i< ITERATIONS; i++) {
      float* current = &data[4*i];
      for (int j = 0; j < 4; j++) {
          data[4*i+ j] = runga_kutta(differentials[j], current, STEP_SIZE, i);
      }
  }

  // TODO: Save to disk
}

/** Applied a single iteration of the Runga Kutta schema to a differential equation.
 * 
 * @param differential: A differential equation pointer whose parameters are p_1, p_2, theta_1, theta_2 respectively. 
 * @param current: Current values for p_1, p_2, theta_1, theta_2. 
 * @param h: Runga Kutta step size
 * @param i: Index of variables, p_1, p_2, theta_1, theta_2, which the differential is in relation to. 0 <= i <= 3
 * 
 */ 
float runga_kutta(float (*differential)(float*), float* current, float h, int i)
// 
{
  float k1 = differential(current);
  current[i] += (k1*h)/2.0;
  float k2 = differential(current);
  current[i] -= (k1*h)/2.0;
  current[i] += (k2*h)/2.0;
  float k3 = differential(current);
  current[i] -= (k2*h)/2.0;
  current[i] += k3*h;
  float k4 = differential(current);
  return current[i] + (h/6) * (k1 + 2*k2+ 2*k3 + k4);
}

/**
 *  Lagragian for p_1, p_2, theta_1, theta_2 respectively. 
 * @param current: Current values for p_1, p_2, theta_1, theta_2 respectively
 * @return: Derivative of variable wrt t. 
 */
float d_p_1(float* current)
{
  float p_1 = current[0]; 
  float p_2 = current[1]; 
  float theta_1 = current[2]; 
  float theta_2 = current[3]; 
  return (-0.5 * MASS * LENGTH) * (d_theta_1(current) * d_theta_2(current) sin(theta_1-theta_2) + 3*(GRAVITY/LENGTH)*sin(theta_1) );
}

float d_p_2(float* current)
{
  float p_1 = current[0]; 
  float p_2 = current[1]; 
  float theta_1 = current[2]; 
  float theta_2 = current[3]; 
  return (-0.5 * MASS * LENGTH) * (-1.0*d_theta_1(current) * d_theta_2(current) sin(theta_1-theta_2) + (GRAVITY/LENGTH)*sin(theta_2) );
}

float d_theta_1(float* current)
{
  float p_1 = current[0]; 
  float p_2 = current[1]; 
  float theta_1 = current[2]; 
  float theta_2 = current[3]; 
  return (6/(MASS*LENGTH*LENGTH)) * ((2*p_1 - 3*p_2* cos(theta_1-theta_2)) / (16-(9*cos(theta_1-theta_2)*cos(theta_1-theta_2))));
}

float d_theta_2(float* current)
{
  float p_1 = current[0]; 
  float p_2 = current[1]; 
  float theta_1 = current[2]; 
  float theta_2 = current[3]; 
  return (6/(MASS*LENGTH*LENGTH)) * ((8*p_2 - 3*p_1* cos(theta_1-theta_2)) / (16-(9*cos(theta_1-theta_2)*cos(theta_1-theta_2))));
}
