# Carbon cluster ionisation
The code simulates the interaction of particles (initially carbons) with a gaussian laser focus. The simulation consists of two stages:

Equilibration: Particles are placed on a grid and interact with each other via Lennard-Jones potential until they reach thermal equilibrium.
Ionization: The particles are exposed to a laser pulse, which can ionize them (remove electrons). The code tracks the positions, velocities, and ionization states of the particles throughout the pulse.


## Installation
```
git clone https://github.com/AnAdisorn/Carbon-cluster-ionization.git
mkdir build
cd build
cmake ..
make
```

## Usage
Initialsing and equilibrating carbon cluster
```
./initialise_cluster <grid_size_x> <grid_size_y> <grid_size_z> [spacing] <steps> <dt>
```
https://github.com/user-attachments/assets/e6147f3f-eb0e-45b2-bcc6-8572f2ac020e

![potential](https://github.com/user-attachments/assets/e67059a8-b6dc-4081-b154-182652c0d670)

Simulating ionising cluster
```
./ionise_cluster <e0> <w> <b> <dt> <cycles> <polarization> <method>
```
https://github.com/user-attachments/assets/8fe7228d-4558-4ebb-bec1-9c2d3e3dd37c

![image](https://github.com/user-attachments/assets/7f084bd9-2ec8-49e2-996c-ab705f2ce2d3)

## Support
Tell people where they can go to for help. It can be any combination of an issue tracker, a chat room, an email address, etc.

## Contributing
State if you are open to contributions and what your requirements are for accepting them.

For people who want to make changes to your project, it's helpful to have some documentation on how to get started. Perhaps there is a script that they should run or some environment variables that they need to set. Make these steps explicit. These instructions could also be useful to your future self.

You can also document commands to lint the code or run tests. These steps help to ensure high code quality and reduce the likelihood that the changes inadvertently break something. Having instructions for running tests is especially helpful if it requires external setup, such as starting a Selenium server for testing in a browser.

## Authors and acknowledgment
Show your appreciation to those who have contributed to the project.

## Project status
If you have run out of energy or time for your project, put a note at the top of the README saying that development has slowed down or stopped completely. Someone may choose to fork your project or volunteer to step in as a maintainer or owner, allowing your project to keep going. You can also make an explicit request for maintainers.
