# Columns UI SDK

The Columns UI SDK provides the interfaces necessary to create panel and toolbar components for Columns UI.

Refer to https://columns-ui-sdk.readthedocs.io for further documentation.

## Building the docs

1. [Install Python 3](https://www.python.org/downloads/)
1. [Install doxygen](http://www.doxygen.nl/download.html#srcbin)
1. Install Python dependencies:
   
    ```shell
    pip install -r docs\requirements.txt
    ``` 
1. Run `doxygen` in the root directory of this repository.
1. Run `docs\make.bat html`

The built docs will be in `docs\build`.
