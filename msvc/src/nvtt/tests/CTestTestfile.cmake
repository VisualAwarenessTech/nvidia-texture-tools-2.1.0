# CMake generated Testfile for 
# Source directory: K:/Development/op3d_active/nvidia-texture-tools-2.1.0/src/nvtt/tests
# Build directory: K:/Development/op3d_active/nvidia-texture-tools-2.1.0/msvc/src/nvtt/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
ADD_TEST(NVTT.TestSuite.Kodak.cuda "nvtestsuite" "-path" "K:/Development/op3d_active/nvidia-texture-tools-2.1.0/data/testsuite" "-set" "0" "-out" "output-cuda-kodak")
ADD_TEST(NVTT.TestSuite.Waterloo.cuda "nvtestsuite" "-path" "K:/Development/op3d_active/nvidia-texture-tools-2.1.0/data/testsuite" "-set" "1" "-out" "output-cuda-waterloo")
ADD_TEST(NVTT.TestSuite.Epic.cuda "nvtestsuite" "-path" "K:/Development/op3d_active/nvidia-texture-tools-2.1.0/data/testsuite" "-set" "2" "-out" "output-cuda-epic")
ADD_TEST(NVTT.TestSuite.Kodak.nocuda "nvtestsuite" "-path" "K:/Development/op3d_active/nvidia-texture-tools-2.1.0/data/testsuite" "-set" "0" "-nocuda" "-out" "output-nocuda-kodak")
ADD_TEST(NVTT.TestSuite.Waterloo.nocuda "nvtestsuite" "-path" "K:/Development/op3d_active/nvidia-texture-tools-2.1.0/data/testsuite" "-set" "1" "-nocuda" "-out" "output-nocuda-waterloo")
ADD_TEST(NVTT.TestSuite.Epic.nocuda "nvtestsuite" "-path" "K:/Development/op3d_active/nvidia-texture-tools-2.1.0/data/testsuite" "-set" "2" "-nocuda" "-out" "output-nocuda-epic")
