import matplotlib.pyplot as plt

# Simulated parameters
current_limit = 0.5  # Amps
initial_current = 0.6  # Amps
max_pot = 255
dt = 46e-6  # 46 microseconds
sim_time = 0.01  # 10ms simulation
steps = int(sim_time / dt)

# PID tuning
Kp = 500
Ki = 80000
Kd = 0.00001

# Internal state
pot = 0
integral = 0
prev_error = 0

# Response data
currents = []
pots = []

for i in range(steps):
    # Simulated DUT: higher pot → lower voltage → lower current
    current = initial_current - (pot / 255.0) * (initial_current - 0.1)

    # PID control only if over limit
    error = current - current_limit
    if error > 0:
        integral += error * dt
        derivative = (error - prev_error) / dt
        prev_error = error
        output = Kp * error + Ki * integral + Kd * derivative
        pot = min(max(pot + output, 0), max_pot)
    else:
        integral = 0
        prev_error = 0
        pot = 0

    currents.append(current)
    pots.append(pot)

# Plot results
time_axis = [i * dt * 1000 for i in range(steps)]
plt.plot(time_axis, currents, label='Current (A)')
plt.plot(time_axis, [current_limit]*steps, 'k--', label='Limit')
plt.plot(time_axis, [p / 255 for p in pots], label='Normalized Pot Value')
plt.xlabel('Time (ms)')
plt.ylabel('Current / Pot')
plt.title('PID Response to Current Limiting')
plt.legend()
plt.grid(True)
plt.show()