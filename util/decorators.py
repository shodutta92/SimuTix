import os
def run_in_env(location):
    def outer(func):
        def inner(*args, **kwargs):
            ORIGINAL_PATH = os.getcwd()
            os.chdir("%s\\%s" % (ORIGINAL_PATH, location))
            func(*args, **kwargs)
            os.chdir(ORIGINAL_PATH)
        return inner
    return outer