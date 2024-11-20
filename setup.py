from setuptools import setup, Extension

module = Extension(
    "symnmfmod",
    sources=["symnmfmodule.c", "symnmf.c"]
)

setup(
    name="symnmfmod",
    version="1.0",
    description="symnmf algorithm in C with Python wrapper",
    ext_modules=[module]
)
