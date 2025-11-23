#include <PSO/PSO.h>

PSO::PSO(const std::function<double(double, double)>& target_function, std::pair<double, double> lower_margin, std::pair<double, double> higher_margin) 
  : target_function(target_function), lower_margin(lower_margin), higher_margin(higher_margin) {}

std::pair<double, double> PSO::solve(int particle_amount, double inertia, double acceleration_coefficient, double v_max) {
  std::vector<Particle> particles(particle_amount);

  std::pair<double, double> global_best;
  double global_best_value = __INT32_MAX__;
  
  for (int i = 0; i < particle_amount; i++)
  {
    particles[i].position.first = double(rand() % 10000) / 10000 * (higher_margin.first - lower_margin.first) + lower_margin.first;
    particles[i].position.second = double(rand() % 10000) / 10000 * (higher_margin.second - lower_margin.second) + lower_margin.second;
    particles[i].velocity.first = double(rand() % 10000) / 10000 * 2 - 1;
    particles[i].velocity.second = double(rand() % 10000) / 10000 * 2 - 1;
    particles[i].personal_best = particles[i].position;
    if (target_function(particles[i].position.first, particles[i].position.second) < global_best_value)
    {
      global_best_value = target_function(particles[i].position.first, particles[i].position.second);
      global_best = particles[i].position;
    }
  }
  
  double velocity_sum = __INT32_MAX__;
  while (particle_amount / 10 < velocity_sum)
  {
    for (int i = 0; i < particle_amount; i++)
    {
      double r1 = rand() % 10000 / 10000.0;
      double r2 = rand() % 10000 / 10000.0;
      double to_p_best_x = particles[i].personal_best.first - particles[i].position.first;
      double to_g_best_x = global_best.first - particles[i].position.first;
      particles[i].velocity.first = inertia * particles[i].velocity.first + acceleration_coefficient * (r1 * to_p_best_x + r2 * to_g_best_x);
      double r3 = rand() % 10000 / 10000.0;
      double r4 = rand() % 10000 / 10000.0;
      double to_p_best_y = particles[i].personal_best.second - particles[i].position.second;
      double to_g_best_y = global_best.second - particles[i].position.second;
      particles[i].velocity.second = inertia * particles[i].velocity.second + acceleration_coefficient * (r3 * to_p_best_y + r4 * to_g_best_y);
      
      particles[i].position.first += particles[i].velocity.first;
      particles[i].position.second += particles[i].velocity.second;
      
      if (target_function(particles[i].position.first, particles[i].position.second) 
          < target_function(particles[i].personal_best.first, particles[i].personal_best.second))
      {
        particles[i].personal_best = particles[i].position;
      }
      if (target_function(particles[i].position.first, particles[i].position.second) < global_best_value)
      {
        global_best_value = target_function(particles[i].position.first, particles[i].position.second);
        global_best = particles[i].position;
      }
    }

    velocity_sum = 0;
    for (int i = 0; i < particle_amount; i++)
    {
      velocity_sum += particles[i].velocity.first + particles[i].velocity.second;
    }
  }

  return global_best;
}

PSOHistory PSO::get_history() {
  return history;
}
