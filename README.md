<a id="readme-top"></a>

<!-- PROJECT NAME -->
<br />
<div align="center">
  <a href="https://github.com/prolibperson/prolib-os">
    <img src="image.png" alt="Logo" width="360" height="231">
  </a>

  <h3 align="center">prolibOS</h3>
  <p align="center">
    An x86 32bit operating system.
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <ul>
        <li><a href="#building">Building</a></li>
        <li><a href="#running">Running</a></li>
      </ul>
    </li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

OS Dev and low level learning project.


<!-- GETTING STARTED -->

### Building

1. Set up a GCC Cross compiler [https://wiki.osdev.org/GCC_Cross-Compiler](https://wiki.osdev.org/GCC_Cross-Compiler)
2. Run make in the root directory of the project
   ```sh
   make
   ```


<!-- USAGE EXAMPLES -->
## Running

- (Using QEMU)
1. Run make run or make run-release in the root directory of the project
   ```sh
   make run
   ```
   ```sh
   make run-release
   ```

- (If you dont wanna use QEMU)
1. Grab an iso in the isobuilds folder and run with whatever VM you want!



<!-- ROADMAP -->
## Roadmap

- [x] Set up interrupts, an IDT and a GDT
- [x] Make a cool looking logging system
- [x] Add keyboard support
- [x] Add Shell
- [ ] Add Custom Bootloader
- [ ] VESA Bios Modes and move away from VGA Text Mode
- [ ] Move to 64-bit
- [ ] FAT-32 Filesystem
- [ ] TCP/IP Stack



<!-- CONTACT -->
## Contact

prolib - [@prolibperson](https://twitter.com/prolibperson) - prolib69420@gmail.com

Project Link: [https://github.com/prolibperson/prolib-os](https://github.com/prolibperson/prolib-os)


<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [OSDev Wiki](https://wiki.osdev.org/)
