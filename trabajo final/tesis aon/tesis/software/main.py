# ------ Module imports ------ #
import logging as log
from comm import Comm

# -------- Local imports --------- #
from app         import App
from app_tabs    import AppTabs
from backend     import Backend
from backend_api import BackendApi
from scheduler   import Scheduler

# -------------- Run ------------- #
def main():

    # Log settings
    log.basicConfig(format='%(asctime)s - %(levelname)s - %(message)s ', datefmt='%Y-%m-%d %H:%M:%S')

    # Instantiate comm
    comm = Comm()

    # Initialize backend api that includes all the other classes
    BackendApi(comm)


if __name__ == "__main__":
    main()
