#!/bin/sh

printf '%s' "did you install git and run 'sudo --validate' first? y/n: "
read ans

if [$ans = "n"]
   printf '%s' "run 'sudo --validate' first!"
   exit 0
fi

sudo apt install gcc make cmake meson dh-autoreconf libxcb-keysyms1-dev libpango1.0-dev libxcb-util0-dev xcb libxcb1-dev libxcb-icccm4-dev libyajl-dev libev-dev libxcb-xkb-dev libxcb-cursor-dev libxkbcommon-dev libxcb-xinerama0-dev libxkbcommon-x11-dev libstartup-notification0-dev libxcb-randr0-dev libxcb-xrm0 libxcb-xrm-dev libxcb-shape0 libxcb-shape0-dev xorg

sudo apt install zsh

git clone https://github.com/ohmyzsh/ohmyzsh.git ~/.oh-my-zsh
cp ~/.oh-my-zsh/templates/zshrc.zsh-template ~/.zshrc

git clone https://github.com/zsh-users/zsh-syntax-highlighting ~/.oh-my-zsh/plugins/zsh-syntax-highlighting

git clone https://github.com/Airblader/i3 i3-gaps
cd i3-gaps && mkdir -p build && cd build
meson .. && ninja

touch .xinitrc && echo "exec i3" >> .xinitrc

printf '%s\n' "next steps: "
printf '%s\n' "   1. change your default shell: 'chsh -s /bin/zsh', then logout and log back in"
printf '%s\n' "   2. add zsh-syntax-highlighting to .zshrc plugins TO THE END OF THE LIST, then run 'source .zshrc'"
printf '%s\n' "   3. add /home/ficko-i3/i3-gaps/build/ to path in zshrc"
printf '%s\n' "   4. logout, then back in and run startx"