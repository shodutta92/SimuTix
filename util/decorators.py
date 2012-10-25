import os
def run_in_env(location):
    def outer(func):
        def inner(*args, **kwargs):
            original_path = os.getcwd()
            os.chdir("%s\\%s" % (original_path, location))
            func(*args, **kwargs)
            os.chdir(original_path)
        return inner
    return outer