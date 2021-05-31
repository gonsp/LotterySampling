import subprocess
import os
import shutil


class BinaryBuilder():

    _current_version_executable_path = None
    _older_versions_executable_paths = None
    _is_repository_freshly_cloned = False

    def build(self, commit=None, profile=None):
        return '../bin/heavy_hitters'
        compilation_config = 'release' if profile is None else 'debug'
        if commit is None:
            if BinaryBuilder._current_version_executable_path is None:
                BinaryBuilder._current_version_executable_path = self._build('../', '../bin/', compilation_config, print_output=True)
            return BinaryBuilder._current_version_executable_path
        else:
            if BinaryBuilder._older_versions_executable_paths is None:
                BinaryBuilder._older_versions_executable_paths = {}
                if os.path.exists('../bin/'):
                    for binary_file in os.listdir('../bin/'):
                        if len(binary_file.split('-')) == 2:
                            BinaryBuilder._older_versions_executable_paths[binary_file.split('-')[1]] = '../bin/' + binary_file

            if commit in BinaryBuilder._older_versions_executable_paths:
                return BinaryBuilder._older_versions_executable_paths[commit]

            if not self._is_repository_freshly_cloned:
                self._clone_repository('.tmp/old_version/')
                self._is_repository_freshly_cloned = True

            self._git_checkout('.tmp/old_version/', commit)
            binary_path = self._build('.tmp/old_version/', '../bin/', compilation_config, suffix='-' + commit)
            BinaryBuilder._older_versions_executable_paths[commit] = binary_path
            return binary_path


    def _build(self, path, output_path, compilation_config, suffix='', print_output=False):
        build_path = path + 'cmake-build-' + compilation_config
        os.makedirs(build_path, exist_ok=True)
        self._run_command('cd ' + build_path + ' && cmake -DCMAKE_BUILD_TYPE=' + compilation_config + ' .. && make', print_output)
        binary_path = output_path + 'heavy_hitters' + suffix
        os.makedirs(output_path, exist_ok=True)
        shutil.copy2(build_path + '/heavy_hitters', binary_path)
        return binary_path


    def _clone_repository(self, path):
        if os.path.isdir(path):
            shutil.rmtree(path)
        os.makedirs(path)
        shutil.copytree('../.git', path + '.git')
        self._run_command('cd ' + path + ' && git checkout .', False)


    def _git_checkout(self, path, commit):
        self._run_command('cd ' + path + ' && git checkout ' + commit, False)


    def _run_command(self, command, print_output):
        pipe = None if print_output else subprocess.DEVNULL
        assert(subprocess.call(command, stdout=pipe, stderr=pipe, shell=True) == 0)