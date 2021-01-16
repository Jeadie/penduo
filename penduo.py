import os
import subprocess

import click
import numpy as np
import matplotlib.animation as animation
import matplotlib.pyplot as plt


@click.group()
def penduo():
    pass

def run_simulation(data):
    result = np.array(list(map(lambda x: [float(i) for i in x.split(",")],  data.readlines())))
    x1, y1 = np.sin(result[:,2]), -1.0 * np.cos(result[:,2])
    x2, y2 = np.sin(result[:,3]) + x1, -1.0 * np.cos( result[:,3]) + y1


    fig = plt.figure()
    ax = fig.add_subplot(111, autoscale_on=False, xlim=(-2, 2), ylim=(-2, 2))
    ax.set_aspect('equal')
    ax.grid()
    line, = ax.plot([], [], 'o-', lw=2)
    time_template = 'time = %.1fs'
    time_text = ax.text(0.05, 0.9, '', transform=ax.transAxes)
    dt = 0.04
    t = np.arange(0, 20, dt)

    def init():
        line.set_data([], [])
        time_text.set_text('')
        return line, time_text


    def animate(i):
        thisx = [0, x1[i], x2[i]]
        thisy = [0, y1[i], y2[i]]

        line.set_data(thisx, thisy)
        time_text.set_text(time_template % (i*dt))
        return line, time_text


    ani = animation.FuncAnimation(fig, animate, range(1, len(y1)),
                                interval=dt*1000, blit=True, init_func=init)
    plt.show()

@penduo.command(help="Visualises the results of a double pendulum simulation.")
@click.option('--data', type=click.File('r') , required=True, help="Double pendulum results from main.c")
def visualise(data):
    run_simulation(data)
    

@penduo.command(help="")
@click.option('--initial-conditions', default ="0.0,0.0,0.0,0.0", help="")
@click.option("--step-size", default = 0.01, help="")
@click.option("--iterations", default = 100, help="")
@click.option("--file-path", default="results.csv", help="")    
@click.option("--do-visualise", default=False, help="")    
def simulate(initial_conditions, step_size, iterations, file_path, do_visualise):
    initial_conditions = initial_conditions.split(" ")
    if len(initial_conditions) != 4:
        print(
            f"There must be four initial variables. Initial conditions were: {initial_conditions}."
        )
        return 1
    print(
        f"Running simulation with following parameters: \n"
        f"  Initial Conditions: \n"
        f"    p_1     = {initial_conditions[0]}\n"
        f"    p_2     = {initial_conditions[1]}\n"
        f"    theta_1 = {initial_conditions[2]}\n"
        f"    theta_2 = {initial_conditions[3]}\n"
        f"\n"
        f"  Step size = {step_size}\n"
        f"  Iterations = {iterations}\n"
        f"  Output paths = {file_path}\n"
    )
    p = subprocess.Popen(
        f"./main "
        f"--initial-conditions {' '.join([str(x) for x in initial_conditions])} "
        f"--step-size={step_size} "
        f"--iterations {iterations} "
        f"--file-path {file_path}".split(),
        stdout=subprocess.PIPE)
    output, error = p.communicate()
    if error and len(error) > 0:
        print(
            f"Error occured when running simulation in C. Error: {error}"
        )
    if do_visualise:
        run_simulation(open(file_path, "r"))


if __name__ == '__main__':
    penduo(obj={})
