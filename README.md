<p align="center">
  <img src="https://www.especial.gr/wp-content/uploads/2019/03/panepisthmio-dut-attikhs.png" alt="UNIWA" width="150"/>
</p>

<p align="center">
  <strong>UNIVERSITY OF WEST ATTICA</strong><br>
  SCHOOL OF ENGINEERING<br>
  DEPARTMENT OF COMPUTER ENGINEERING AND INFORMATICS
</p>

<p align="center">
  <a href="https://www.uniwa.gr" target="_blank">University of West Attica</a> ·
  <a href="https://ice.uniwa.gr" target="_blank">Department of Computer Engineering and Informatics</a>
</p>

---

<p align="center">
  <strong>Operating Systems II</strong>
</p>

<h1 align="center">
  Parallel Inner Product Calculation using C POSIX Threads
</h1>

<p align="center">
  <strong>Vasileios Evangelos Athanasiou</strong><br>
  Student ID: 19390005
</p>

<p align="center">
  <a href="https://github.com/Ath21" target="_blank">GitHub</a> ·
  <a href="https://www.linkedin.com/in/vasilis-athanasiou-7036b53a4/" target="_blank">LinkedIn</a>
</p>

<hr/>

<p align="center">
  <strong>Supervision</strong>
</p>

<p align="center">
  Supervisor: Vasileios Mamalis, Professor
</p>
<p align="center">
  <a href="https://ice.uniwa.gr/en/emd_person/vassilios-mamalis/" target="_blank">UNIWA Profile</a>
</p>

<p align="center">
  Co-supervisor: Nikolaos Psarras, Applications Lecturer<br>
</p>

<p align="center">
  <a href="https://ice.uniwa.gr/en/emd_person/nikolaos-psarras/" target="_blank">UNIWA Profile</a> ·
  <a href="https://www.linkedin.com/in/psarras-nikolas-20234183/" target="_blank">LinkedIn</a>
</p>

</hr>

---

<p align="center">
  Athens, June 2022
</p>

---

<p align="center">
  <img src="https://www.softprayog.in/wp-content/uploads/2018/10/pthreads-in-c-under-linux-1.webp" width="250"/>
</p>

---

# README

## Parallel Inner Product Calculation using C POSIX Threads

This repository contains an assignment for **Operating Systems II**, focusing on parallel computation using POSIX threads in C to efficiently calculate the inner product of two vectors.

---

## Table of Contents

| Section | Folder/File                           | Description                                     |
| ------: | ------------------------------------- | ----------------------------------------------- |
|       1 | `assign/`                             | Assignment material for the Threads workshop    |
|     1.1 | `assign/ASK-2A-OS-II-LAB-2021-22.png` | Assignment description in English               |
|     1.2 | `assign/ΑΣΚ-2Α-ΛΣ-ΙΙ-ΕΡΓ-2021-22.png` | Assignment description in Greek                 |
|       2 | `src/`                                | Source code demonstrating thread usage          |
|     2.1 | `src/pthreads.c`                      | POSIX threads (pthreads) example implementation |
|       3 | `README.md`                           | Project documentation                           |
|       4 | `INSTALL.md`                          | Usage instructions                              |

---

## 1. Project Overview

The project implements a C program that calculates the **inner product** of two vectors using multiple threads.

The inner product is defined as:

A₁ × B₁ + A₂ × B₂ + ... + Aₙ × Bₙ

where Aᵢ and Bᵢ are elements of vectors A and B.

The computation is parallelized by dividing the workload among multiple threads.

---

## 2. Objectives

- Compute the inner product using **p parallel threads**.
- Assign each thread a portion of the vector to compute.
- Combine partial sums into a shared global result.
- Protect shared data using mutex synchronization.
- Measure performance with different thread counts (1, 2, 4, and 8).
- Support vector input via user input or file loading.

---

## 3. Key Features

- **Dynamic Thread Creation**  
  Number of threads is determined at runtime.

- **Local Computation**  
  Each thread computes a local partial sum independently.

- **Mutex Synchronization**  
  Critical sections are protected to prevent race conditions.

- **Random Vector Initialization**  
  Allows performance testing with large datasets.

---

## 4. Program Structure

1. Read vector size `n` and thread count `p`.
2. Allocate memory for vectors A and B.
3. Create threads to compute partial products.
4. Synchronize updates to shared variable `total_sum`.
5. Output the final inner product result.
