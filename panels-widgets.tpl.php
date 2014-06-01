<?php
/**
 * @file
 * Default theme implementation for panels widgets.
 *
 * Available variables:
 * - $content: The renderrable content items. Use render($content) to print
 *   the content in its entirety.
 * - $title: The panels widget type label.
 * - $classes: String of classes applied to the panels widget entity.
 */
?>
<div class="<?php print $classes; ?>">
  <?php print render($content); ?>
</div>
