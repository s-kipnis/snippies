
# place in cmk_update_agent.py

import os
import sys
import warnings

# disable annoying warning in pyca cryptography openssl bindings
if sys.platform == "win32" and os.environ.get("PROCESSOR_ARCHITEW6432") is not None:
    warnings.filterwarnings("ignore", category=UserWarning)

