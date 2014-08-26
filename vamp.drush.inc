<?php

/**
 * Implementation of hook_drush_command().
 */
function vamp_drush_command() {

  $items['vamp-connect'] = array(
    'description' => 'Ssh into a remote server',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('vssh'),
    'arguments' => array(
      'alias' => 'the alias to ssh into',
    ),
    'examples' => array(
      'drush vamp-connect @svdhout.labs',
      'drush vssh @svdhout.labs',
    ),
  );

  return $items;
}


/**
 * Implementation of drush_hook_COMMAND().
 */
function drush_vamp_connect($alias) {

  // bypass the default remote host system using the alias as a parameter instead off a real alias
  $site = drush_sitealias_get_record($alias);

  // We only accept remote aliases.
  if (empty($site['connect-host'])) {
    drush_set_error('DRUSH_SITE_CONNECT_HOST_ALIAS_REQUIRED', dt('@alias does not specify a connect-host.', array('@alias' => $alias)));
    return;
  }

  $pw = "";
  if (isset($site['connect-pw'])) {
    $pw = " -p" . $site['connect-pw'];
  }
  $cmd = "sshpass" . $pw . " ssh " . $site['connect-user'] . "@" . $site['connect-host'];

    
  $status = drush_shell_proc_open($cmd);
  if ($status != 0) {
    return drush_set_error('DRUSH_SITE_CONNECT_ERROR', dt('An error @code occurred while running the command `@command`', array('@command' => $cmd, '@code' => $status)));
  }
}

