# Distributed Programming Examples

This directory contains examples of distributed programming model using the TT-Metalium API.

Users familiar with the single-device TT-Metal programming model will find the distributed programming model to be a natural extension.

## 1. Program Dispatch

The `program_dispatch` directory contains an example of broadcasting a program across a mesh of devices.

## 2. Unified Distributed Memory: Distributed Read-Write

The `unified_distributed_memory` directory contains an example of R/W from/to a distributed memory buffer spanning multiple devices in a mesh. The example
demonstrates how to allocate a L1 MeshBuffer, and perform R/W from/to it using the `MeshCommandQueue`.

## 3. Mesh-Native Programming Model: Distributed Program Dispatch + Distributed Memory Space

The `distributed_eltwise_add` directory contains an example of using the mesh-native programming model to perform distributed element-wise addition.
