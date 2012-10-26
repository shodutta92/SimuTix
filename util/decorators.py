import os
def run_in_env(location):
    def outer(func):
        def inner(*args, **kwargs):
            ORIGINAL_PATH = os.getcwd()
            os.chdir("%s\\%s" % (ORIGINAL_PATH, location))
            ret = func(*args, **kwargs)
            os.chdir(ORIGINAL_PATH)
            return ret
        return inner
    return outer