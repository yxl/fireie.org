<?php print $picture ?>

<div class="<?php print $classes . ' ' . $zebra; ?>"<?php print $attributes; ?>>

  <div class="clearfix">

  <?php if ($new) : ?>
    <span class="new"><?php print drupal_ucfirst($new) ?></span>
  <?php endif; ?>
  
	<span class="submitted"><?php print $submitted ?></span>
    <?php if (theme_get_setting('show_comment_title','corporateclean')): ?>
      <?php print render($title_prefix); ?>
      <h3<?php print $title_attributes; ?>><?php print $title ?></h3>
      <?php print render($title_suffix); ?>
    <?php endif; ?>

    <div class="content"<?php print $content_attributes; ?>>
      <?php hide($content['links']); print render($content); ?>
      <?php if ($signature): ?>
      <div class="clearfix">
        <div>—</div>
        <?php print $signature ?>
      </div>
      <?php endif; ?>
    </div>
  </div>

  <?php print render($content['links']) ?>
</div>