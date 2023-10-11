TITLE="Información del sistema para $HOSTNAME"
RIGHT_NOW=$(date +"%x %r%Z")
TIME_STAMP="Actualizada el $RIGHT_NOW por $USER"

##### Estilos

TEXT_ULINE=$(tput sgr 0 1)
TEXT_BOLD=$(tput bold)
TEXT_GREEN=$(tput setaf 2)
TEXT_RESET=$(tput sgr0)

##### Funciones

system_info()
{
  echo "${TEXT_ULINE}Versión del sistema${TEXT_RESET}"
  uname -a
  echo
}


show_uptime()
{
  echo "${TEXT_ULINE}Tiempo de encendido del sistema${TEXT_RESET}"
  uptime
  echo
}


drive_space()
{
  echo "${TEXT_ULINE}Particiones del disco${TEXT_RESET}"
  df -h
  echo
}


home_space()
{
  echo "${TEXT_ULINE}Tamaño de los directorios del usuario${TEXT_RESET}"
  if [ "$USER" != root ]; then
      echo "USAGE   DIRECTORIO"
      du -hd0 /home/$USER | sort -nr
  else
      sudo du -hd0 /home/* | sort -nr
  fi
}

##### Programa principal

cat << _EOF_
$TEXT_BOLD$TITLE$TEXT_RESET

$TEXT_GREEN$TIME_STAMP$TEXT_RESET
_EOF_

system_info
show_uptime
drive_space
home_space