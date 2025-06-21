import os, re, unittest

# Каталоги с исходниками
SRC_DIRS = [
    os.path.join(os.path.dirname(__file__), os.pardir, 'src', 'web'),
]

# Список обязательных маршрутов и файлов, в которых они должны встречаться
REQUIRED_ROUTES = {
    '/api/config/import': 'routes_config.cpp',
    '/api/v1/config/export': 'routes_config.cpp',
}

class RoutePresenceTest(unittest.TestCase):
    def test_required_routes_exist(self):
        for route, expected_file in REQUIRED_ROUTES.items():
            found = False
            for src_dir in SRC_DIRS:
                for root, _dirs, files in os.walk(src_dir):
                    for fname in files:
                        if fname.endswith('.cpp') and expected_file in fname:
                            path = os.path.join(root, fname)
                            with open(path, 'r', encoding='utf-8') as f:
                                if route in f.read():
                                    found = True
                                    break
                    if found:
                        break
                if found:
                    break
            self.assertTrue(found, f"Route '{route}' not found in {expected_file}")

if __name__ == '__main__':
    unittest.main() 