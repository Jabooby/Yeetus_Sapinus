

# Installation d'application pour RPi
 - Pour GitHub desktop
	- mettre le fichier terminant en ".deb" dans Downloads
		- Copier les prochaines lignes dans le terminal:
			- `cd Downloads`
			- `sudo dpkg -i GitHubDesktop-linux-*-linux1.deb`
			- `github-desktop`
 - Pour connecter un appareil Bluetooth
	- Ouvrez un terminal
		- Copier les prochaines lignes dans le terminal:
			- `sudo bluetoothctl`
			- `agent on`
			- `default agent`
    		- `scan on`
			- `devices`
    		- `pair <MAC ADDRESS>`
			- `connect <MAC ADDRESS>`
    		- `trust <MAC ADDRESS>`
			- `scan off`
			-  `exit`
