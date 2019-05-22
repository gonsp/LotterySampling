import subprocess
import os
import shutil


class BinaryBuilder():

    _current_version_executable_path = None
    _older_versions_executable_paths = None

    def build(self, commit=None, profile=None):
        compilation_config = "release" if profile is None else "debug"
        if commit is None:
            if BinaryBuilder._current_version_executable_path is None:
                BinaryBuilder._current_version_executable_path = self._build('../', '../bin/', compilation_config)
            return BinaryBuilder._current_version_executable_path
        else:
            if BinaryBuilder._older_versions_executable_paths is None:
                self._clone_repository(".tmp/old_version/")
                BinaryBuilder._older_versions_executable_paths = {}
            elif commit in BinaryBuilder._older_versions_executable_paths:
                return BinaryBuilder._older_versions_executable_paths[commit]

            self._git_checkout(".tmp/old_version/", commit)
            binary_path = self._build('.tmp/old_version/', '../bin/', compilation_config, suffix=commit)
            BinaryBuilder._older_versions_executable_paths[commit] = binary_path
            return binary_path


    def _build(self, path, output_path, compilation_config, suffix=""):
        print('BUILDING EXECUTABLE')
        build_path = path + 'cmake-build-' + compilation_config
        os.makedirs(build_path, exist_ok=True)
        self._run_command('cd ' + build_path + ' && cmake -DCMAKE_BUILD_TYPE=' + compilation_config + ' .. && make')
        binary_path = output_path + "heavy_hitters" + suffix
        os.makedirs(output_path, exist_ok=True)
        shutil.copy2(build_path + '/heavy_hitters', binary_path)
        return binary_path


    def _clone_repository(self, path):
        if os.path.isdir(path):
            shutil.rmtree(path)
        os.makedirs(path)
        shutil.copytree("../.git", path + ".git")
        self._run_command("cd " + path + " && git checkout .")


    def _git_checkout(self, path, commit):
        self._run_command("cd " + path + " && git checkout " + commit)


    def _run_command(self, command):
        process = subprocess.Popen(command, shell=True)
        process.communicate()
        assert(process.returncode == 0)