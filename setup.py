from setuptools import setup, Extension

module = Extension(
    "symnmf_mod",
    sources=["symnmfmodule.c"]
)

setup(
    name="symnmf_mod",
    version="1.0",
    description="symnmf algorithm in C with Python wrapper",
    ext_modules=[module]
)
