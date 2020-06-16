git remote set-url origin "https://gitlab.com/pgimeno/irrlicht-mirror.git"
git fetch -p origin
git pull --no-verify
git remote set-url origin "https://github.com/slater1/irrlicht.git"
git push --no-verify --mirror
