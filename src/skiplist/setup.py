from setuptools import setup, Extension
import pybind11

cpp_args = ['-std=c++20', '-stdlib=libc++', '-mmacosx-version-min=10.7']

sfc_module = Extension(
    'stringskiplist',
    sources=['main.cpp'],
    include_dirs=[pybind11.get_include()],
    language='c++',
    extra_compile_args=cpp_args,
)

setup(
    name='stringskiplist',
    version='1.0',
    description='Python package with stringskiplist C++ extension (PyBind11)',
    ext_modules=[sfc_module],
)